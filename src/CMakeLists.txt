set(EXEC_NAME study_opengl)
set(DIR_GLAD ${DIR_3RDPARTY}/glad)
set(DIR_GLFW ${DIR_3RDPARTY}/glfw)
set(DIR_STB ${DIR_3RDPARTY}/stb)
set(DIR_GLM ${DIR_3RDPARTY}/glm)
set(DIR_ASSIMP ${DIR_3RDPARTY}/assimp)



configure_file(${DIR_SRC}/helper/config.h.in
        ${DIR_SRC}/helper/config.h
        @ONLY NEWLINE_STYLE UNIX)


aux_source_directory(${DIR_SRC}/shader SRC_SHADER_FILES)
aux_source_directory(${DIR_SRC}/example SRC_EXAMPLE_FILES)
aux_source_directory(${DIR_SRC}/src01 SRC_01_FILES)
aux_source_directory(${DIR_SRC}/src02 SRC_02_FILES)
aux_source_directory(${DIR_SRC}/src03 SRC_03_FILES)
aux_source_directory(${DIR_SRC}/src04 SRC_04_FILES)
aux_source_directory(${DIR_SRC}/src05 SRC_05_FILES)
aux_source_directory(${DIR_SRC}/helper SRC_HELPER_FILES)
aux_source_directory(${DIR_SRC}/gui SRC_GUI_FILES)

add_executable(${EXEC_NAME}
        ${SRC_SHADER_FILES}
        ${SRC_EXAMPLE_FILES}
        ${SRC_01_FILES}
        ${SRC_02_FILES}
        ${SRC_03_FILES}
        ${SRC_04_FILES}
        ${SRC_05_FILES}
        ${SRC_HELPER_FILES}
        ${SRC_GUI_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)


target_include_directories(${EXEC_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${DIR_3RDPARTY}
        ${DIR_GLAD}/include
        ${DIR_GLFW}/include
        ${DIR_STB}
        ${DIR_GLM}
        ${DIR_ASSIMP}/include
        ${DIR_3RDPARTY}/assimp_lib/include)

target_link_libraries(${EXEC_NAME} PRIVATE
        glad::static
        glfw::static
        imgui::static
        assimp)

target_link_directories(${EXEC_NAME} PRIVATE
        ${DIR_3RDPARTY}/assimp_lib)
