#include <iostream>
#include <string>
#include <glad/gl.h>// GLAD
#include <GLFW/glfw3.h>// GLFW (include after glad)
#include <memory>
#include "Window.h"
#include "Shape.h"
#include "shaderClass.h"

// This example is taken from http://learnopengl.com/
// http://learnopengl.com/code_viewer.php?code=getting-started/hellowindow2
// The code originally used GLEW, I replaced it with Glad

//https://github.com/k41531/opengl-mac/blob/main/VAO.h
//https://tokoik.github.io/GLFWdraft.pdf

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

int main(){

    glfwInit();// Init GLFW

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwSwapInterval(1);// 垂直同期のタイミングを待つ
    
    // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0){
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    Window window;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);// 背景色を指定する
    glViewport(0, 0, WIDTH, HEIGHT);// Define the viewport dimensions

    auto vertexFile = "./shaders/point.vert", fragFile = "./shaders/point.frag";
	Shader shaderProgram(vertexFile, fragFile);

    // uniform 変数の場所を取得する
    const GLint sizeLoc(glGetUniformLocation(shaderProgram.ID, "size"));
    const GLint scaleLoc(glGetUniformLocation(shaderProgram.ID, "scale"));
    const GLint locationLoc(glGetUniformLocation(shaderProgram.ID, "location"));
   // 矩形の頂点の位置
    constexpr Object::Vertex rectangleVertex[] =
    {
    { -0.5f, -0.5f },
    { 0.5f, -0.5f },
    { 0.5f, 0.5f },
    { -0.5f, 0.5f }
    };
    
    std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));// 図形データを作成する

    while (window){
        
        glClear(GL_COLOR_BUFFER_BIT);// ウィンドウを消去する
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        shaderProgram.Activate();// シェーダプログラムの使用開始    

         // uniform 変数に値を設定する
        glUniform2fv(sizeLoc, 1, window.getSize());
        glUniform1f(scaleLoc, window.getScale());
        glUniform2fv(locationLoc, 1, window.getLocation());
        shape->draw();// 図形を描画する
               
        window.swapBuffers();// カラーバッファを入れ替える
    }
	
	// Delete window before ending the program
	glfwTerminate();// Terminate GLFW before ending the program
    return 0;
}