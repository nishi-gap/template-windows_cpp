#pragma once
#include <iostream>
#include <glad/gl.h>// GLAD
#include <GLFW/glfw3.h>

// ウィンドウ関連の処理
//GLFWAPI GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
/*
    width
    heghit
    title
    monitor:    フルスクリーンでなければNULL
    share:      他のウィンドウとリソース共有、しないならNULL    
 */
class Window
{
 
    GLFWwindow *const window;// ウィンドウのハンドル   
    GLfloat size[2];// ウィンドウのサイズ
    GLfloat scale;// ワールド座標系に対するデバイス座標系の拡大率 
    GLfloat location[2]; // 図形の正規化デバイス座標系上での位置

    int keyStatus;// キーボードの状態
    public:
    // コンストラクタ
    Window(int width = 640, int height = 480, const char *title = ""): 
        window(glfwCreateWindow(width, height, title, NULL, NULL)), scale(100.0f), location{ 0.0f, 0.0f }{
        if (window == NULL){
            std::cerr << "Can't create GLFW window." << std::endl;// ウィンドウが作成できなかった
            exit(1);
        }
        
        glfwMakeContextCurrent(window);// 現在のウィンドウを処理対象にする

        int version = gladLoadGL(glfwGetProcAddress);
        if (version == 0) {
            std::cerr << "Failed to initialize OpenGL context" << std::endl;
            exit(1);
        }
        glfwSwapInterval(1); // 垂直同期のタイミングを待つ   
        glfwSetWindowUserPointer(window, this); // このインスタンスの this ポインタを記録しておく
        glfwSetWindowSizeCallback(window, resize); // ウィンドウのサイズ変更時に呼び出す処理の登録        
        glfwSetScrollCallback(window, wheel);// マウスホイール操作時に呼び出す処理の登録       
        glfwSetKeyCallback(window, keyboard); // キーボード操作時に呼び出す処理の登録
        resize(window, width, height); // 開いたウィンドウの初期設定
    }

    // デストラクタ
    virtual ~Window(){
        glfwDestroyWindow(window);
    }

     // 描画ループの継続判定
    explicit operator bool(){
       
        glfwPollEvents();// イベントを取り出す
        
        // マウスの左ボタンの状態を調べる
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE){
            double x, y;// マウスの左ボタンが押されていたらマウスカーソルの位置を取得する
            glfwGetCursorPos(window, &x, &y);
            // マウスカーソルの正規化デバイス座標系上での位置を求める
            location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
            location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
        } 

        // キーボードの状態を調べる
        if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)location[0] -= 2.0f / size[0];
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)location[0] += 2.0f / size[0];
        if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)location[1] -= 2.0f / size[1];
        else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)location[1] += 2.0f / size[1];

        return !glfwWindowShouldClose(window)&& !glfwGetKey(window, GLFW_KEY_ESCAPE);// ウィンドウを閉じる必要がなければ true を返す
    }

    // ダブルバッファリング
    void swapBuffers() const{
        glfwSwapBuffers(window);// カラーバッファを入れ替える
    }

     // ウィンドウのサイズ変更時の処理
    static void resize(GLFWwindow *const window, int width, int height){     
        int fbWidth, fbHeight; // フレームバッファのサイズを調べる
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);// フレームバッファ全体をビューポートに設定する
        
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));// このインスタンスの this ポインタを得る
        if (instance != NULL){
            // 開いたウィンドウのサイズを保存する
            instance->size[0] = static_cast<GLfloat>(width);
            instance->size[1] = static_cast<GLfloat>(height);
        }
     }

      // マウスホイール操作時の処理
    static void wheel(GLFWwindow *window, double x, double y){
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));// このインスタンスの this ポインタを得る
            if (instance != NULL){
                instance->scale += static_cast<GLfloat>(y);// ワールド座標系に対するデバイス座標系の拡大率を更新する
            }
    }

     // キーボード操作時の処理
    static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods){     
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));// このインスタンスの this ポインタを得る
        if (instance != NULL) instance->keyStatus = action; // キーの状態を保存する        
    }
    
    const GLfloat *getSize() const { return size; }// ウィンドウのサイズを取り出す
    GLfloat getScale() const { return scale; } // ワールド座標系に対するデバイス座標系の拡大率を取り出す
    const GLfloat *getLocation() const { return location; }// 位置を取り出す
   
};