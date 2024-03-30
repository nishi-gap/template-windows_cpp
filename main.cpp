#include <iostream>
#include <string>
#include <glad/gl.h>// GLAD
#include <GLFW/glfw3.h>// GLFW (include after glad)
#include <memory>
#include "Window.h"
#include "Shape.h"
#include "shaderClass.h"
#include "Matrix.h"
#include <Eigen/Dense>

// This example is taken from http://learnopengl.com/
// http://learnopengl.com/code_viewer.php?code=getting-started/hellowindow2
// The code originally used GLEW, I replaced it with Glad

//https://github.com/k41531/opengl-mac/blob/main/VAO.h
//https://tokoik.github.io/GLFWdraft.pdf

using namespace Eigen;
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

int main(){

    glfwInit();// Init GLFW

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    
    Window window;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);// 背景色を指定する

    std::cout << std::filesystem::current_path() << std::endl;
    auto vertexFile = "../shaders/point.vert", fragFile = "../shaders/point.frag";
	Shader shaderProgram(vertexFile, fragFile);

    // uniform 変数の場所を取得する
    const GLint modelviewLoc(glGetUniformLocation(shaderProgram.ID, "modelview"));
    const GLint projectionLoc(glGetUniformLocation(shaderProgram.ID, "projection"));

   // 八面体の頂点の位置
constexpr Object::Vertex octahedronVertex[] =
{
 { 0.0f, 1.0f, 0.0f },
 { -1.0f, 0.0f, 0.0f },
 { 0.0f, -1.0f, 0.0f },
 { 1.0f, 0.0f, 0.0f },
 { 0.0f, 1.0f, 0.0f },
 { 0.0f, 0.0f, 1.0f },
 { 0.0f, -1.0f, 0.0f },
 { 0.0f, 0.0f, -1.0f },
 { -1.0f, 0.0f, 0.0f },
 { 0.0f, 0.0f, 1.0f },
 { 1.0f, 0.0f, 0.0f },
 { 0.0f, 0.0f, -1.0f }
};
    
    std::unique_ptr<const Shape> shape(new Shape(3, 12, octahedronVertex));// 図形データを作成する
    Eigen::Matrix4d M_scale = Eigen::Matrix4d::Identity(), 
    M_trans = Eigen::Matrix4d::Identity(), model = Eigen::Matrix4d::Identity();
    while (window){
        
        glClear(GL_COLOR_BUFFER_BIT);// ウィンドウを消去する
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        shaderProgram.Activate();// シェーダプログラムの使用開始    
        
        // 拡大縮小の変換行列を求める
        const GLfloat *const size(window.getSize());
        const GLfloat scale = window.getScale() * 2.0;      
        const GLfloat fovy(window.getScale() * 0.01);
        const GLfloat aspect(size[0] / size[1]);
        const GLfloat w(size[0] / scale), h(size[1] / scale);
        const GLfloat *const position(window.getLocation()); // 平行移動の変換行列を求める
        
        M_scale(0,0) = scale / size[0]; M_scale(1,1) = scale / size[1];
        M_trans(0,3) = position[0]; M_trans(1,3) = position[1]; 

        model = M_trans; // モデル変換行列を求める

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