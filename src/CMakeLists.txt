set(TARGET_NAME kubarem_lib)

file(GLOB_RECURSE KUBAREM_SOURCE_FILES
     *.c
     *.cpp)

file(GLOB_RECURSE KUBAREM_HEADER_FILES
     *.h
     *.hpp)

add_library(kubarem_lib ${KUBAREM_SOURCE_FILES} ${KUBAREM_HEADER_FILES})
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${TARGET_NAME} PUBLIC ${kubarem_SOURCE_DIR}/src)
target_include_directories(${TARGET_NAME} PUBLIC "${FREETYPE_INCLUDE_DIR}")
target_include_directories(${TARGET_NAME} PUBLIC "${GLFW_INCLUDE_DIR}")
target_include_directories(${TARGET_NAME} PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(${TARGET_NAME} PUBLIC "${GLM_INCLUDE_DIR}")
target_include_directories(${TARGET_NAME} PUBLIC "${IMGUI_INCLUDE_DIR}")
target_include_directories(${TARGET_NAME} PUBLIC "${STB_IMAGE_INCLUDE_DIR}")
target_include_directories(${TARGET_NAME} PUBLIC "${SOLOUD_INCLUDE_DIR}")
target_include_directories(${TARGET_NAME} PUBLIC "${ENTT_INCLUDE_DIR}")
target_include_directories(${TARGET_NAME} PUBLIC "${YAML_CPP_INCLUDE_DIR}")

target_link_libraries(${TARGET_NAME} "${OPENGL_LIBRARIES}")
target_link_libraries(${TARGET_NAME} "${GLEW_LIBRARIES}")
target_link_libraries(${TARGET_NAME} "${DEFAULT_LIBRARIES}")
target_link_libraries(${TARGET_NAME} "${OPENGL_LIBRARY}")
target_link_libraries(${TARGET_NAME} "${FREETYPE_LIBRARY}")
target_link_libraries(${TARGET_NAME} "${GLFW_LIBRARY}")
target_link_libraries(${TARGET_NAME} "${CMAKE_DL_LIBS}")
target_link_libraries(${TARGET_NAME} "${GLAD_LIBRARY}")
target_link_libraries(${TARGET_NAME} "${ASSIMP_LIBRARY}")
target_link_libraries(${TARGET_NAME} "${IMGUI_LIBRARY}")
target_link_libraries(${TARGET_NAME} "${STB_IMAGE_LIBRARY}")
target_link_libraries(${TARGET_NAME} "${SOLOUD_LIBRARY}")
target_link_libraries(${TARGET_NAME} "${ENTT_LIBRARY}")
target_link_libraries(${TARGET_NAME} "${YAML_CPP_LIBRARY}")

source_group("sources" FILES ${SOURCE_FILES_EXE})
source_group("headers" FILES ${HEADER_FILES_EXE})
