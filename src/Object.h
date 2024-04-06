#pragma once
#include <glad/gl.h>
#include <Eigen/Dense>
#include <memory>
#include "Geometry.h"

// 図形データ
namespace GLObject{
    class Object:public std::enable_shared_from_this<Object>{

    GLuint vao;// 頂点配列オブジェクト名
    GLuint vbo;// 頂点バッファオブジェクト名
    GLuint ibo;// インデックスの頂点バッファオブジェクト
    public:
        // 頂点属性
        struct drawVertex{
            GLfloat position[3];// 位置      
            GLfloat color[3]; // 色
            drawVertex(const Eigen::Vector3d& v);
            drawVertex(const Eigen::Vector3d& v, const Eigen::Vector3d& c);
            
            void addColor(Eigen::Vector3d c);
        };
        // コンストラクタ
        Object(const std::vector<std::shared_ptr<GeoElement::Vertex> >& vertices);

        virtual ~Object();
        private:
        
        Object(const Object &o);// コピーコンストラクタによるコピー禁止
        Object &operator=(const Object &o);// 代入によるコピー禁止
        public:
        // 頂点配列オブジェクトの結合
        void bind()const; 
        void unbind();// Unbinds the VAO
        void generate(const std::vector<drawVertex>& vertices, const std::vector<GLuint>& indices);
    };

    // 図形の描画
    class Shape{
        std::shared_ptr<const Object> object;// 図形データ
        protected:
        GLsizei vertexcount; // 描画に使う頂点の数
        public:
        Shape(const std::vector<std::shared_ptr<GeoElement::Vertex> >& vertices);
        
        void draw() const;
        
        // 描画の実行
        virtual void execute() const;
    };

    // インデックスを使った図形の描画
    class ShapeIndex : public Shape{
    protected:
        GLsizei indexcount;// 描画に使う頂点の数
    public:
        // コンストラクタ
        // vertices: 頂点属性を格納した配列
        ShapeIndex(const std::vector<std::shared_ptr<GeoElement::Vertex> >& vertices): 
            Shape(vertices), indexcount(vertices.size()){}
        virtual void execute() const;// 描画の実行
    };

    class SolidShapeIndex: public ShapeIndex{
    public:
        SolidShapeIndex(const std::vector<std::shared_ptr<GeoElement::Vertex> >& vertices): ShapeIndex(vertices){}
        virtual void execute() const;// 描画の実行
    };
}
