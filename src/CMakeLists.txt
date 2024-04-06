cmake_minimum_required(VERSION 3.29)

add_library(SRC_DIR  ArcBall.cpp Geometry.cpp Matrix.cpp Object.cpp  shaderClass.cpp)
target_link_libraries (SRC_DIR Eigen3::Eigen)# 実行ファイルにリンクするライブラリの指定
#target_include_directories(SRC_DIR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})# ヘッダファイルのインクルードパスの指定
