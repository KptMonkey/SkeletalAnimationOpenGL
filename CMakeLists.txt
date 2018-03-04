cmake_minimum_required(VERSION 2.8)
#specify project
project( Animation )
SET(CMAKE_CXX_FLAGS "-std=c++14")
# The following folder will be included
include_directories(${PROJECT_SOURCE_DIR})
include_directories(
	src
)

add_executable(animation ${PROJECT_SOURCE_DIR}/src/main.cpp
                ${PROJECT_SOURCE_DIR}/src/RenderAbstraction
                ${PROJECT_SOURCE_DIR}/src/Texture
                ${PROJECT_SOURCE_DIR}/src/main
                ${PROJECT_SOURCE_DIR}/src/Mesh
                ${PROJECT_SOURCE_DIR}/src/RenderContext
                ${PROJECT_SOURCE_DIR}/src/VertexArray
                ${PROJECT_SOURCE_DIR}/src/Camera
                ${PROJECT_SOURCE_DIR}/src/Shader
                ${PROJECT_SOURCE_DIR}/src/FrameBuffer
                ${PROJECT_SOURCE_DIR}/src/AnimationController
                ${PROJECT_SOURCE_DIR}/src/Animation
               )
