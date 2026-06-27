cmake_minimum_required(VERSION 3.16)
project(glprofiler LANGUAGES CXX)

add_library(glprofiler STATIC
    src/profiler.cpp
)
add_library(glp::profiler ALIAS glprofiler)

target_include_directories(glprofiler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(glprofiler PUBLIC cxx_std_17)

set(GLP_GL_TARGET "" CACHE STRING "CMake target that provides the GL loader headers")
set(GLP_GL_INCLUDE_DIRS "" CACHE STRING "include dirs for the GL loader header")
set(GLP_GL_HEADER "" CACHE STRING "override for the GL loader header")

if(GLP_GL_TARGET AND TARGET ${GLP_GL_TARGET})
    target_link_libraries(glprofiler PRIVATE ${GLP_GL_TARGET})
endif()

if(GLP_GL_INCLUDE_DIRS)
    target_include_directories(glprofiler PRIVATE ${GLP_GL_INCLUDE_DIRS})
endif()

if(GLP_GL_HEADER)
    target_compile_definitions(glprofiler PRIVATE GLP_GL_HEADER=${GLP_GL_HEADER})
endif()
