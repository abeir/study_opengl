set(GLFW_LIBRARY_TYPE STATIC)

add_subdirectory(glfw)

add_library(glfw::static ALIAS glfw)

