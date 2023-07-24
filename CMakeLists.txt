cmake_minimum_required(VERSION 3.20)

project(corgi-opengl DESCRIPTION "ProjectDescription" LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_DEBUG_POSTFIX d)

set(deps_folder "C:/deps")
set(SDL_FOLDER "C:/deps/SDL2-2.28.1")

if(MSVC)
	add_library(SDL2 SHARED IMPORTED GLOBAL SYSTEM)

	SET_PROPERTY(TARGET SDL2 
		PROPERTY IMPORTED_LOCATION ${SDL_FOLDER}/lib/x64/SDL2.dll)
	SET_PROPERTY(TARGET SDL2 
		PROPERTY IMPORTED_IMPLIB  ${SDL_FOLDER}/lib/x64/SDL2.lib)
	SET_PROPERTY(TARGET SDL2 
		PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${SDL_FOLDER}/include)

	add_library(SDL2main SHARED IMPORTED GLOBAL SYSTEM)

	SET_PROPERTY(TARGET SDL2main 
		PROPERTY IMPORTED_IMPLIB  ${SDL_FOLDER}/lib/x64/SDL2main.lib)
	SET_PROPERTY(TARGET SDL2main 
		PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${SDL_FOLDER}/include)

    add_subdirectory(deps/glad)

endif()




find_package(corgi-math CONFIG)

add_library(${PROJECT_NAME} STATIC "")

target_link_libraries(${PROJECT_NAME} PUBLIC glad corgi-math)

# Setting warning level. WX and Werror means warnings are treated as errors.
if(MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE -W4 -WX)
else()
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)
endif()

set_property(TARGET ${PROJECT_NAME}  PROPERTY CXX_STANDARD 23)

target_include_directories(${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

add_subdirectory(src)

# Targets we want to export and where
install(TARGETS ${PROJECT_NAME} glad EXPORT ${PROJECT_NAME}Targets
    LIBRARY     DESTINATION lib
    RUNTIME     DESTINATION bin)

# We move the include files to their destination 
install(DIRECTORY include DESTINATION ./)

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion)

install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME}
)


# Tests

option(BUILD_TESTS "Build the tests" ON)

if(BUILD_TESTS)
enable_testing()
add_subdirectory(tests)
endif()