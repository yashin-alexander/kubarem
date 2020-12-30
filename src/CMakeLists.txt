set(TARGET_NAME kubarem)

file(GLOB_RECURSE SOURCE_FILES_EXE
     *.c
     *.cpp)

file(GLOB_RECURSE HEADER_FILES_EXE
     *.h
     *.hpp)

add_executable(${TARGET_NAME} ${HEADER_FILES_EXE} ${SOURCE_FILES_EXE})
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET_NAME} PRIVATE ${kubarem_SOURCE_DIR}/src)

target_include_directories(${PROJECT_NAME} PUBLIC "${FREETYPE_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLM_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${IMGUI_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${STB_IMAGE_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${SOLOUD_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${ENTT_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME} "${OPENGL_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} "${GLEW_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} "${DEFAULT_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} "${OPENGL_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${FREETYPE_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${GLFW_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME} "${GLAD_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${ASSIMP_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${STB_IMAGE_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${SOLOUD_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${ENTT_LIBRARY}")

source_group("sources" FILES ${SOURCE_FILES_EXE})
source_group("headers" FILES ${HEADER_FILES_EXE})
