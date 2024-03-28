#pragma once
#include <memory>
#include "Object.h"// 図形データ
// 図形の描画
class Shape{
    std::shared_ptr<const Object> object;// 図形データ
    protected:
    const GLsizei vertexcount; // 描画に使う頂点の数
    public:
    // コンストラクタ
    // size: 頂点の位置の次元
    // vertexcount: 頂点の数
    // vertex: 頂点属性を格納した配列
    Shape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex): 
    object(new Object(size, vertexcount, vertex)),vertexcount(vertexcount){}
    void draw() const{
        object->bind();// 頂点配列オブジェクトを結合する
        execute();// 描画を実行する
    }
    // 描画の実行
    virtual void execute() const{
        glDrawArrays(GL_LINE_LOOP, 0, vertexcount); // 折れ線で描画する
    }
};