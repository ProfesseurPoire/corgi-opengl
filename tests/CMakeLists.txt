cmake_minimum_required (VERSION 3.13.0)

project(test-corgi-opengl)

add_executable(${PROJECT_NAME} WIN32  "") 

# I'm not sure I understand all the advantages on using target_sources but at least
# it's one way to avoid adding variables, and to define our target early
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp )

target_link_libraries(${PROJECT_NAME} corgi-opengl SDL2 SDL2main)

set_property(TARGET ${PROJECT_NAME}  PROPERTY CXX_STANDARD 20)

add_subdirectory(unit_tests)

