cmake_minimum_required (VERSION 3.13.0)

project(unit_tests-corgi-opengl)

find_package(corgi-test CONFIG)

add_executable(${PROJECT_NAME} "src/unit_tests_main.cpp") 


target_link_libraries(${PROJECT_NAME} corgi-opengl corgi-test SDL2 SDL2main)

set_property(TARGET ${PROJECT_NAME}  PROPERTY CXX_STANDARD 20)

add_test( NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})


