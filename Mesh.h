#include <Eigen/Dense>
#include <memory>
#include <vector>
#include <glad/gl.h>

//https://mitani.cs.tsukuba.ac.jp/lecture/jikken_old/polygon_operation.pdf
class Face;
class HalfEdge;

class Vertex{
public:
    Eigen::Vector3d p;
    std::vector<std::shared_ptr<HalfEdge>> halfEdge;
    Vertex(Eigen::Vector3d _p):p(_p), halfedge({}){}
};

class HalfEdge{
public:
    std::shared_ptr<HalfEdge> next;
    std::shared_ptr<HalfEdge> prev;
    std::shared_ptr<HalfEdge> pair;
    std::shared_ptr<Vertex> v;
    std::shared_ptr<Face> face;
    HalfEdge(std::shared_ptr<Vertex> _v):v(_v){
        if(v != nullptr)v->halfEdge.push_back(this);
    }
};

class Face{
public:
    std::shared_ptr<HalfEdge> halfEdge;
    Face(std::shared_ptr<HalfEdge> _halfEdge):halfEdge(_halfEdge){}
};

class Mesh{
public:
    std::vector<std::shared_ptr<Vertex>> vertices;
    std::vector<std::shared_ptr<Face>> faces;
private:
    void setHalfEdgePair(const std::shared_ptr<HalfEdge>& h){
        for(const auto& f: faces){
            auto he = f->halfEdge;
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
    void setHalfEdgePair(const std::shared_ptr<HalfEdge>& h1, const std::shared_ptr<HalfEdge>& h2){
        h1->pair = h2;
        h2->pair = h1;
    }
    void deleteVertex(const std::shared_ptr<Vertex>& v){
        auto itr = std::find(vertices.begin(), vertices.end(), v);
        if(itr != vertices.end())vertices.erase(itr);
    }
    //halfedgeの削除が正しいのか要検証
    void deleteFace(const std::shared_ptr<Face>& f){
        auto itr_f = std::find(faces.begin(), faces.end(), f);
        if(itr_f != faces.end()){
            //faceを構築するhalfEdgeを削除
            for(auto itr_he = f->halfEdge; itr_he != f->halfEdge; itr_he = itr_he->next){
                auto itr_he2 = std::find(itr_he->v->halfEdge.begin(), itr_he->v->halfEdge.end(), itr_he);
                if(itr_he2 != itr_he->v->halfEdge.end())itr_he->v->halfEdge.erase(itr_he2);
            }
            faces.erase(itr_f);
        }
    }   
public:
    void addFace(const std::vector<std::shared_ptr<Vertex>>& vertices){
        std::vector<std::shared_ptr<HalfEdge>> halfEdges;
        for(auto&v: vertices)halfEdges.push_back(std::make_shared<HalfEdge>(v));
        for(int i = 0; i < (int)halfEdges.size(); i++){
            halfEdges[i]->next = halfEdges[(i + 1) % halfEdges.size()];
            halfEdges[i]->prev = halfEdges[(i - 1 + halfEdges.size()) % halfEdges.size()];
        }
        std::shared_ptr<Face> face = std::make_shared<Face>(halfEdges[0]);
        faces.push_back(face);
        for(auto&he: halfEdges)he->face = face;
        
        for(auto&he: halfEdges) setHalfEdgePair(he);
        
    }
};