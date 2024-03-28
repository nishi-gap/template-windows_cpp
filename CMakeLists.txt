cmake_minimum_required(VERSION 3.12)

# プロジェクト名の設定
project(sample_glad)

# 必須ライブラリの存在チェック
find_package(glfw3 REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
#find_package(glm REQUIRED)

# glad関係
include_directories(glad/include)
add_subdirectory(glad)

set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")# リソースファイルのディレクトリを指定
file(GLOB SHADER_SOURCES ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.vert)# シェーダーファイルの取得

# シェーダーファイルのコピー
foreach(SHADER_FILE ${SHADER_SOURCES})
    # リソースファイルをビルドディレクトリにコピー
    configure_file(${SHADER_FILE} ${CMAKE_BINARY_DIR}/shaders COPYONLY)
endforeach()

add_compile_options(-O2 -Wall)# コンパイルオプション
add_executable(sample_glad main.cpp ${SHADER_SOURCES} shaderClass.cpp)# 実行ファイルの指定

# 実行ファイルにリンクするライブラリの指定
#target_link_libraries(sample_glad glad glfw glm ${CMAKE_DL_LIBS})
target_link_libraries(sample_glad glad glfw ${CMAKE_DL_LIBS})
target_link_libraries (sample_glad Eigen3::Eigen)

# c++20を使う
set_property(TARGET sample_glad PROPERTY CXX_STANDARD 20)


