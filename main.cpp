#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <Eigen/Dense>
#include <glad/gl.h>// GLAD
#include <GLFW/glfw3.h>// GLFW (include after glad)

#include "Window.h"
#include "./src/shaderClass.h"
#include "./src/Matrix.h"
#include "./src/Geometry.h"
#include "./src/Object.h"

// This example is taken from http://learnopengl.com/
// http://learnopengl.com/code_viewer.php?code=getting-started/hellowindow2
// The code originally used GLEW, I replaced it with Glad

//https://github.com/k41531/opengl-mac/blob/main/VAO.h
//https://tokoik.github.io/GLFWdraft.pdf

using namespace Eigen;
std::shared_ptr<GeoElement::Mesh> loadOBJ(const std::string& filename);

int main(){

    glfwInit();// Init GLFW

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    Window window;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);// 背景色を指定する

    // 背面カリングを有効にする
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    // デプスバッファを有効にする
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    std::string vertexFile = "../shaders/point.vert", fragFile = "../shaders/point.frag";
	Shader shaderProgram(vertexFile, fragFile);

    std::string objfile = "../cube.obj";
    std::shared_ptr<GeoElement::Mesh> mesh = loadOBJ(objfile);
    auto loadvertices = mesh->getTriangleMeshes();
    
    // uniform 変数の場所を取得する
    const GLint modelviewLoc(glGetUniformLocation(shaderProgram.ID, "modelview"));
    const GLint projectionLoc(glGetUniformLocation(shaderProgram.ID, "projection"));

    std::unique_ptr<const GLObject::Shape> shape(new GLObject::SolidShapeIndex(loadvertices)); // 図形データを作成する
    Eigen::Matrix4d M_trans = Eigen::Matrix4d::Identity(), model = Eigen::Matrix4d::Identity();

    glfwSetTime(0.0);// タイマーを 0 にセット
    while (window){
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        shaderProgram.Activate();// シェーダプログラムの使用開始    
        
        // 拡大縮小の変換行列を求める
        const GLfloat *const size(window.getSize());
        //const GLfloat scale = window.getScale() * 2.0;      
        const GLfloat fovy(window.getScale() * 0.01);
        const GLfloat aspect(size[0] / size[1]);
        const GLfloat *const position(window.getLocation()); // 平行移動の変換行列を求める
        auto r = getRotate(static_cast<double>(glfwGetTime()), Eigen::Vector3d(0,1,0));
        //M_scale(0,0) = scale / size[0]; M_scale(1,1) = scale / size[1];
        M_trans(0,3) = position[0]; M_trans(1,3) = position[1]; 

        model = M_trans * r; // モデル変換行列を求める

        // ビュー変換行列を求める
        Eigen::Vector3d eye(3,3,3), center(0,0,0), up(0,1,0);
        Eigen::Matrix4d view = getLookAt(eye, center, up);
        std::vector<GLfloat> view_data = Mat2Array(view * model);
        std::vector<GLfloat> prj_data = Mat2Array(getPerspective(fovy,aspect, 1.0f, 10.0));
        
        // uniform 変数に値を設定する
        glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, view_data.data());
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, prj_data.data());

        shape->draw();// 図形を描画する
               
        window.swapBuffers();// カラーバッファを入れ替える
    }
	
	// Delete window before ending the program
	glfwTerminate();// Terminate GLFW before ending the program
    return 0;
}

std::shared_ptr<GeoElement::Mesh> loadOBJ(const std::string& filename) {
        std::ifstream file(filename);
        if (!file)  throw std::runtime_error("Failed to open file: " + filename);
        
        std::shared_ptr<GeoElement::Mesh> mesh = std::make_shared<GeoElement::Mesh>();
        std::vector<std::shared_ptr<GeoElement::Vertex> > vertices;
        std::vector<Eigen::Vector3d> normals;
        std::vector<std::vector<int> > face_vertices_index, face_normal_index;
        std::string line;
        int vcnt = 0;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") { // Vertex
                Eigen::Vector3d vertex;
                iss >> vertex[0] >> vertex[1] >> vertex[2];
                vertices.push_back(std::make_shared<GeoElement::Vertex>(vertex, vcnt++));
            }else if(prefix == "vn"){
                Eigen::Vector3d vertex;
                iss >> vertex[0] >> vertex[1] >> vertex[2];
                normals.push_back(vertex);
            }
            else if (prefix == "f") { // Face
                std::vector<int> vertexIndices, normalIndices;
                char slash; // To skip slashes

                while (iss.good()) {
                    int vertexIndex, normalIndex;
                    iss >> vertexIndex >> slash; // Read vertex index
                    iss.ignore(std::numeric_limits<std::streamsize>::max(), '/'); // Skip texture coordinate
                    iss >> normalIndex; // Read normal index

                    vertexIndices.push_back(--vertexIndex);
                    normalIndices.push_back(--normalIndex);
                }
                face_vertices_index.push_back(vertexIndices);
                face_normal_index.push_back(normalIndices);
            }
        }
        for(const auto& find: face_vertices_index){
            std::vector<std::shared_ptr<GeoElement::Vertex> > _vertices;
            for(const auto& index: find){
                _vertices.push_back(vertices[index]);
            }
            mesh->addFace(_vertices);
        }
        return mesh;
    }
