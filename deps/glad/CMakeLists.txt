cmake_minimum_required(VERSION 3.20)

project(glad DESCRIPTION "ProjectDescription" LANGUAGES C VERSION 1.0.0)

add_library(glad STATIC src/glad.c)

set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)

target_include_directories(glad PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)