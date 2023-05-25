cmake_minimum_required(VERSION 3.26)
project(study_opengl)

set(CMAKE_CXX_STANDARD 17)

set(DIR_SRC ${PROJECT_SOURCE_DIR}/src)
set(DIR_3RDPARTY ${PROJECT_SOURCE_DIR}/3rdparty)
set(DIR_RES ${PROJECT_SOURCE_DIR}/res)

add_subdirectory(3rdparty)
add_subdirectory(src)