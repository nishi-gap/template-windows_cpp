#pragma once
#include <Eigen/Dense>
#include <memory>
#include <vector>
#include<fstream>
#include<sstream>
#include <numbers>
#include <glad/gl.h>

//https://mitani.cs.tsukuba.ac.jp/lecture/jikken_old/polygon_operation.pdf

namespace GeoElement{
    class Face;
    class HalfEdge;

    class Vertex{
    public:
        Eigen::Vector3d p;
        int id;
        std::vector<std::shared_ptr<HalfEdge> > halfEdge;
        Vertex(Eigen::Vector3d _p, int _id);
        void bindHalfEdge(const std::shared_ptr<HalfEdge>& he);
    };

    class HalfEdge : public std::enable_shared_from_this<HalfEdge>{
    public:
        std::shared_ptr<HalfEdge> next;
        std::shared_ptr<HalfEdge> prev;
        std::shared_ptr<HalfEdge> pair;
        std::shared_ptr<Vertex> v;
        std::shared_ptr<Face> face;
        HalfEdge(std::shared_ptr<Vertex> _v);
    };

    class Face{
    public:
        std::shared_ptr<HalfEdge> halfEdge;
        Face(std::shared_ptr<HalfEdge> _halfEdge);

        //ear clippingアルゴリズムによる単純な実装
        //入力が凸多角形であることを仮定
        std::vector<std::vector<std::shared_ptr<Vertex> > > splitTriangles();
        private:
        double VectorArea(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2);
        bool isPointInTriangle(const std::shared_ptr<Vertex>& point, const std::vector<std::shared_ptr<Vertex> >& triangle);
    
        //右手形で判定する
        //直線AOと直線OBのなす角が180度未満かどうか
        bool isLessThanPi(const Eigen::Vector3d& a, const Eigen::Vector3d& o, const Eigen::Vector3d& b, const Eigen::Vector3d& Plane);
    };

    class Mesh{
    public:
        std::vector<std::shared_ptr<Vertex> > vertices;
        std::vector<std::shared_ptr<Face> > faces;
    private:
        void setHalfEdgePair(const std::shared_ptr<HalfEdge>& h);
        void setHalfEdgePair(const std::shared_ptr<HalfEdge>& h1, const std::shared_ptr<HalfEdge>& h2);
        void deleteVertex(const std::shared_ptr<Vertex>& v);
        void deleteFace(const std::shared_ptr<Face>& f);
    public:
        void addFace(const std::vector<std::shared_ptr<Vertex> >& vertices);
        std::vector<std::shared_ptr<Vertex> > getTriangleMeshes();
    };
}
