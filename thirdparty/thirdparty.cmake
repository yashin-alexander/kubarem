# OpenGL
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
set(OPENGL_LIBRARY ${OPENGL_LIBRARIES})

# freetype2
set(FREETYPE_DIR "${THIRDPARTY_DIR}/freetype")

add_subdirectory("${FREETYPE_DIR}")

set(FREETYPE_LIBRARY "freetype")
set(FREETYPE_INCLUDE_DIR "${FREETYPE_DIR}/include")

MESSAGE(STATUS "FREETYPE_LIBRARY = ${FREETYPE_LIBRARY}")
MESSAGE(STATUS "FREETYPE_INCLUDE_DIR = ${FREETYPE_INCLUDE_DIR}")

# glfw
find_library(GLFW_LIBRARY "glfw" "/usr/lib" "/usr/local/lib")
find_path(GLFW_INCLUDE_DIR "glfw/glfw.h" "/usr/include" "/usr/local/include")

if((NOT GLFW_LIBRARY) OR (NOT GLFW_INCLUDE_DIR))
	set(GLFW_DIR "${THIRDPARTY_DIR}/glfw")

	set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "Build the GLFW documentation")
	set(GLFW_INSTALL        OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory("${GLFW_DIR}")

	set(GLFW_LIBRARY "glfw" "${GLFW_LIBRARIES}")
	set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")
endif()

# glad
set(GLAD_DIR "${THIRDPARTY_DIR}/glad")
add_library("glad" STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")

set(GLAD_LIBRARY "glad")
set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")

# glm
set(GLM_DIR "${THIRDPARTY_DIR}/glm")
execute_process(COMMAND git submodule update --init ${GLM_DIR}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(GLM_INCLUDE_DIR "${GLM_DIR}")

# assimp
set(ASSIMP_DIR "${THIRDPARTY_DIR}/assimp")
execute_process(COMMAND git submodule update --init ${ASSIMP_DIR}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(ASSIMP_INCLUDE_DIR "${ASSIMP_DIR}/include")
set(ASSIMP_LIBRARY "${ASSIMP_DIR}/bin/libassimp.so.5.0.1")

# stb-image
set(STB_IMAGE_DIR "${THIRDPARTY_DIR}/stb-image")
add_library("stb-image" STATIC "${STB_IMAGE_DIR}/stb_image.cpp")
target_include_directories("stb-image" PRIVATE "${STB_IMAGE_DIR}")

set(STB_IMAGE_LIBRARY "stb-image")
set(STB_IMAGE_INCLUDE_DIR "${STB_IMAGE_DIR}")
