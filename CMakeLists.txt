cmake_minimum_required(VERSION 3.20)


include(FetchContent)

FetchContent_Declare(
  corgi-math
  GIT_REPOSITORY https://github.com/ProfesseurPoire/corgi-math
  GIT_TAG        "develop")

FetchContent_MakeAvailable(corgi-math)


project(corgi-opengl DESCRIPTION "ProjectDescription" LANGUAGES CXX VERSION 1.0.0)

get_target_property(LIBA_INCLUDES corgi-math INCLUDE_DIRECTORIES)

# Construct the compiler string for the include directories.
foreach(dir ${LIBA_INCLUDES} )
    message(warning ${dir} )
endforeach()

set(CMAKE_DEBUG_POSTFIX d)

add_library(${PROJECT_NAME} STATIC "")

find_package(SDL2 REQUIRED)
find_package(glad REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2main glad::glad corgi-math)

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
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
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