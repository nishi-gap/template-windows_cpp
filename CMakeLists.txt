cmake_minimum_required(VERSION 3.29)

# プロジェクト名の設定
project(sample_glad)

# 必須ライブラリの存在チェック
find_package(glfw3 REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# glad関係
include_directories(glad/include)
add_subdirectory(glad)

add_subdirectory(src)# 外部ファイルのディレクトリを追加

set(SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")# リソースファイルのディレクトリを指定
file(GLOB SHADER_SOURCES ${SHADER_DIR}/*.frag ${SHADER_DIR}/*.vert)# シェーダーファイルの取得

# シェーダーファイルのコピー
foreach(SHADER_FILE ${SHADER_SOURCES})
    # リソースファイルをビルドディレクトリにコピー
    configure_file(${SHADER_FILE} ${CMAKE_BINARY_DIR}/shaders COPYONLY)
endforeach()

add_compile_options(-O2 -Wall)# コンパイルオプション
add_executable(sample_glad main.cpp ${SHADER_SOURCES})# 実行ファイルの指定

# 実行ファイルにリンクするライブラリの指定
target_link_libraries(sample_glad glad glfw ${CMAKE_DL_LIBS})
target_link_libraries (sample_glad Eigen3::Eigen SRC_DIR)

# c++20を使う
#set_property(TARGET sample_glad PROPERTY CXX_STANDARD 20)
# C++の標準規格の指定
target_compile_features(sample_glad PUBLIC cxx_std_20)

