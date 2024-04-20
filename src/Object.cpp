#include "Object.h"
#include <iostream>

void GLObject::Object::drawVertex::addColor(Eigen::Vector3d c){
    c = c.normalized();
    
    color[0] = std::abs(static_cast<GLfloat>(c.x()));
    color[1] = std::abs(static_cast<GLfloat>(c.y()));
    color[2] = std::abs(static_cast<GLfloat>(c.z()));
}

GLObject::Object::drawVertex::drawVertex(const Eigen::Vector3d& v){
    position[0] = static_cast<GLfloat>(v.x());
    position[1] =  static_cast<GLfloat>(v.y());
    position[2] = static_cast<GLfloat>(v.z());
    color[0] = 0.f; color[1] =  0.f; color[2] =  0.f;
}

GLObject::Object::drawVertex::drawVertex(const Eigen::Vector3d& v, const Eigen::Vector3d& c){
    position[0] = static_cast<GLfloat>(v.x());
    position[1] =  static_cast<GLfloat>(v.y());
    position[2] = static_cast<GLfloat>(v.z());
    color[0] = static_cast<GLfloat>(c.x());
    color[1] =  static_cast<GLfloat>(c.y());
    color[2] =  static_cast<GLfloat>(c.z());
}

GLObject::Object::Object(const std::vector<std::shared_ptr<GeoElement::Vertex> >& vertices){
        std::vector<drawVertex> v;
        std::vector<GLuint> indices;
        for(const auto& vertex: vertices){
            v.push_back(drawVertex(vertex->p));
            indices.push_back(static_cast<GLint>(vertex->id));
        }
        
        for(int i = 0; i < (int)vertices.size(); i += 3){
            Eigen::Vector3d normal = (vertices[i + 1]->p - vertices[i]->p).cross(vertices[i + 2]->p - vertices[i]->p);
            for(int j = 0; j < 3; j++)v[i + j].addColor(normal);
        }
        indices[0] = 0; indices[1] = 1; indices[2] = 2;
        generate(v, indices);
    }

GLObject::Object::~Object(){
    glDeleteVertexArrays(1, &vao); // 頂点配列オブジェクトを削除する
    glDeleteBuffers(1, &vbo);// 頂点バッファオブジェクトを削除する   
    glDeleteBuffers(1, &ibo);// インデックスの頂点バッファオブジェクトを削除する
}

void GLObject::Object::bind()const{ glBindVertexArray(vao);/*描画する頂点配列オブジェクトを指定する*/ }
    
// Unbinds the VAO
void GLObject::Object::unbind(){
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLObject::Object::generate(const std::vector<drawVertex>& vertices, const std::vector<GLuint>& indices){
    // 頂点配列オブジェクト
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // 頂点バッファオブジェクト
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof (drawVertex), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // 結合されている頂点バッファオブジェクトを in 変数から参照できるようにする
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(drawVertex), reinterpret_cast<void*>(offsetof(drawVertex, position)));
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (drawVertex), static_cast<drawVertex *>(0)->position);

    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof (drawVertex), static_cast<drawVertex *>(0)->color);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(drawVertex), reinterpret_cast<void*>(offsetof(drawVertex, color)));

    
    // インデックスの頂点バッファオブジェクト
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);//unbind vao
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);   //  <- インデックスバッファのアンバインド (vaoのアンバインドより後ろに)
    
}

GLObject::Shape::Shape(const std::vector<std::shared_ptr<GeoElement::Vertex> >& vertices){
    object = std::make_shared<Object>(vertices);
    std::vector<int> IDnoCollision, indices;
    for(const auto& vertex: vertices){
        if(std::find(IDnoCollision.begin(), IDnoCollision.end(), vertex->id) == IDnoCollision.end())IDnoCollision.push_back(vertex->id);
        indices.push_back(static_cast<GLuint>(vertex->id));
    }
   vertexcount = static_cast<GLsizei>(indices.size());
}

GLObject::ShapeIndex::ShapeIndex(const std::vector<std::shared_ptr<GeoElement::Vertex> >& vertices): 
    Shape(vertices), indexcount(static_cast<GLsizei>(vertices.size())){}
    

void GLObject::Shape::draw() const{
    object->bind();// 頂点配列オブジェクトを結合する
    
    this->execute();
}

void GLObject::Shape::execute() const{
    glDrawArrays(GL_LINE_LOOP, 0, vertexcount); // 折れ線で描画する
}

void GLObject::ShapeIndex::execute() const{
    glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);// 線分群で描画する
}

void GLObject::SolidShapeIndex::execute() const{
    //glDrawElements(GL_TRIANGLE_STRIP, 2*indexcount, GL_UNSIGNED_INT, 0);// 三角形で描画する
    glDrawArrays(GL_TRIANGLES, 0, indexcount); // 折れ線で描画する
}