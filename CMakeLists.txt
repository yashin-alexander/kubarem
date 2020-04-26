cmake_minimum_required(VERSION 3.9.2 FATAL_ERROR)
project(kubarem)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

include(thirdparty/thirdparty.cmake)

add_subdirectory(src)
