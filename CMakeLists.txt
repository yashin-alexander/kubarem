cmake_minimum_required(VERSION 3.9.2 FATAL_ERROR)
project(kubarem)
set(TARGET_NAME kubarem)

execute_process(COMMAND git submodule update --init --recursive)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

include(thirdparty/thirdparty.cmake)

add_executable(${TARGET_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "${FREETYPE_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLFW_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLM_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${IMGUI_INCLUDE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${STB_IMAGE_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME} "${OPENGL_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} "${GLEW_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} ${DEFAULT_LIBRARIES})
target_link_libraries(${PROJECT_NAME} "${OPENGL_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${FREETYPE_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${GLFW_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${CMAKE_DL_LIBS}")
target_link_libraries(${PROJECT_NAME} "${GLAD_LIBRARY}")


add_library(logger src/log.c src/log.h)
target_link_libraries(${PROJECT_NAME} logger)


#target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -pedantic -Werror)
