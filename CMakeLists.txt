cmake_minimum_required(VERSION 3.9.2 FATAL_ERROR)
project(kubarem)

execute_process(COMMAND git submodule update --init --recursive)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

add_executable(kubarem src/main.cpp)
include(thirdparty/thirdparty.cmake)
add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC "${FREETYPE_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLM_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${IMGUI_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${STB_IMAGE_INCLUDE_DIR}")

target_link_libraries(kubarem ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${DEFAULT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} "${OPENGL_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${FREETYPE_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${GLFW_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${GLAD_LIBRARY}" "${CMAKE_DL_LIBS}")
