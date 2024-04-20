#include "Geometry.h"
namespace GeoElement{
    Vertex::Vertex(Eigen::Vector3d _p, int _id){
        p = _p; id = _id;
        halfEdge = std::vector<std::shared_ptr<HalfEdge> >();
    }
    void Vertex::bindHalfEdge(const std::shared_ptr<HalfEdge>& he){
        if(std::find(halfEdge.begin(), halfEdge.end(), he) == halfEdge.end())halfEdge.push_back(he);
    }

    HalfEdge::HalfEdge(std::shared_ptr<Vertex> _v){
        next = nullptr; prev = nullptr; pair = nullptr; face = nullptr; v = _v;
    }

    Face::Face(std::shared_ptr<HalfEdge> _halfEdge){ halfEdge = _halfEdge;}
    std::vector<std::vector<std::shared_ptr<Vertex> > > Face::splitTriangles() {
        std::vector<std::shared_ptr<Vertex> > vertices;
        std::vector<std::vector<std::shared_ptr<Vertex> > > triangles;
        std::shared_ptr<HalfEdge> he = halfEdge;
        do{
            vertices.push_back(he->v);
            he = he->next;
        }while(he != halfEdge);

        while ((int)vertices.size() > 3) {
            for (size_t i = 0; i < vertices.size(); i++) {
                size_t prev = (i + vertices.size() - 1) % vertices.size();
                size_t next = (i + 1) % vertices.size();
                std::vector<std::shared_ptr<Vertex> > triangle(3);
                triangle[0] = vertices[prev]; triangle[1] = vertices[i]; triangle[2] = vertices[next];
                // Check if the triangle is an ear
                bool isEar = true;
                for (const auto& vertex : vertices) {
                    if(std::find(triangle.begin(), triangle.end(), vertex) != triangle.end())continue;
                    bool res2 = isPointInTriangle(vertex, triangle);
                    if (res2) {
                        isEar = false; break;
                    }
                }

                if (isEar) {
                    triangles.push_back(triangle);
                    vertices.erase(vertices.begin() + i);
                    break;
                }
            }
        }
        if (vertices.size() == 3)  triangles.push_back(vertices);// Add the remaining triangle       
        return triangles;
    }

    double Face::VectorArea(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2){
        Eigen::Vector3d crossVec = 0.5 * v1.cross(v2);
        return crossVec.norm();
    }
    bool Face::isPointInTriangle(const std::shared_ptr<Vertex>& point, const std::vector<std::shared_ptr<Vertex> >& triangle) {
        double OriginalArea = VectorArea(triangle[0]->p - triangle[1]->p, triangle[0]->p - triangle[2]->p);
        double area1 = VectorArea(triangle[0]->p - point->p, triangle[1]->p - point->p);
        double area2 = VectorArea(triangle[1]->p - point->p, triangle[2]->p - point->p);
        double area3 = VectorArea(triangle[0]->p - point->p, triangle[2]->p - point->p);
        double err = std::abs(area1 + area2 + area3 - OriginalArea);
        return err < 1e-5? true: false;
    }
    //右手形で判定する
    //直線AOと直線OBのなす角が180度未満かどうか
    bool Face::isLessThanPi(const Eigen::Vector3d& a, const Eigen::Vector3d& o, const Eigen::Vector3d& b, const Eigen::Vector3d& Plane){
        Eigen::Vector3d dir = (a - o).cross(b - o);
        return dir.dot(Plane) > 0;
    }
    
    void Mesh::setHalfEdgePair(const std::shared_ptr<HalfEdge>& h){
        for(const std::shared_ptr<Face>& f: faces){
            std::shared_ptr<HalfEdge> he = f->halfEdge;
            do{
                if(he->v == h->next->v && he->next->v == h->v){
                    h->pair = he;
                    he->pair = h;
                    return;
                }
                he = he->next;
            }while(he != f->halfEdge);
        }
    }
    void Mesh::setHalfEdgePair(const std::shared_ptr<HalfEdge>& h1, const std::shared_ptr<HalfEdge>& h2){
        h1->pair = h2; h2->pair = h1;
    }
    void Mesh::deleteVertex(const std::shared_ptr<Vertex>& v){
        std::vector<std::shared_ptr<Vertex> >::iterator itr = std::find(vertices.begin(), vertices.end(), v);
        if(itr != vertices.end())vertices.erase(itr);
    }
    //halfedgeの削除が正しいのか要検証
    void Mesh::deleteFace(const std::shared_ptr<Face>& f){
        std::vector<std::shared_ptr<Face> >::iterator itr_f = std::find(faces.begin(), faces.end(), f);
        if(itr_f != faces.end()){
            //faceを構築するhalfEdgeを削除
            for(auto itr_he = f->halfEdge; itr_he != f->halfEdge; itr_he = itr_he->next){
                auto itr_he2 = std::find(itr_he->v->halfEdge.begin(), itr_he->v->halfEdge.end(), itr_he);
                if(itr_he2 != itr_he->v->halfEdge.end())itr_he->v->halfEdge.erase(itr_he2);
            }
            faces.erase(itr_f);
        }
    }   
    void Mesh::addFace(const std::vector<std::shared_ptr<Vertex> >& vertices){
        std::vector<std::shared_ptr<HalfEdge> > halfEdges;
        for(auto& v: vertices){
            if(std::find(this->vertices.begin(), this->vertices.end(), v) == this->vertices.end())this->vertices.push_back(v);
            std::shared_ptr<HalfEdge> h = std::make_shared<HalfEdge>(v);
            v->bindHalfEdge(h);
            halfEdges.push_back(h);
        }
        for(int i = 0; i < (int)halfEdges.size(); i++){
            halfEdges[i]->next = halfEdges[(i + 1) % halfEdges.size()];
            halfEdges[i]->prev = halfEdges[(i - 1 + halfEdges.size()) % halfEdges.size()];
        }
        std::shared_ptr<Face> face = std::make_shared<Face>(halfEdges[0]);
        faces.push_back(face);
        for(std::shared_ptr<HalfEdge>& he: halfEdges)he->face = face;
        for(std::shared_ptr<HalfEdge>& he: halfEdges) setHalfEdgePair(he);      
    }

    std::vector<std::shared_ptr<Vertex> > Mesh::getTriangleMeshes() {
        std::vector<std::shared_ptr<Vertex> > triangles;
        for (const std::shared_ptr<Face>& face : faces) {
            auto faceTriangles = face->splitTriangles();
            //triangles = faceTriangles[0];
            for(const auto& triangle: faceTriangles)triangles.insert(triangles.end(), triangle.begin(), triangle.end());
        }
        return triangles;
    }

    void Mesh::moveCenter(){
        Eigen::Vector3d sum(0,0,0);
        for(const std::shared_ptr<Vertex>& v: vertices)sum += v->p;
        Eigen::Vector3d center = sum / vertices.size();
        for(const std::shared_ptr<Vertex>& v: vertices)v->p -= center;
    }

    void Mesh::getSphere(double& r, Eigen::Vector3d& center){
        Eigen::Vector3d sum(0,0,0);
        for(const std::shared_ptr<Vertex>& v: vertices)sum += v->p;
        center = sum / vertices.size();
        r = 0.0;
        for(const std::shared_ptr<Vertex>& v: vertices){
            double dist = (v->p - center).norm();
            if(dist > r)r = dist;
        }
        
    }
}