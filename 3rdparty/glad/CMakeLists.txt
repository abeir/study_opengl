project(glad)

set(DIR_GLAD ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} STATIC
        ${DIR_GLAD}/src/glad.c)

target_include_directories(${PROJECT_NAME} PUBLIC ${DIR_GLAD}/include)