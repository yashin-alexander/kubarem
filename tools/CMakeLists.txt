set(EDITOR_APP_NAME editor)

# editor

file(GLOB_RECURSE KUBAREM_EDITOR_SOURCE_FILES
        *.c
        *.cpp)

file(GLOB_RECURSE KUBAREM_EDITOR_HEADER_FILES
        *.h
        *.hpp)

add_executable(${EDITOR_APP_NAME} ${KUBAREM_EDITOR_SOURCE_FILES} ${KUBAREM_EDITOR_HEADER_FILES})
target_link_libraries(${EDITOR_APP_NAME} kubarem_lib)
set_property(TARGET ${EDITOR_APP_NAME} PROPERTY CXX_STANDARD 17)
set_target_properties(${EDITOR_APP_NAME} PROPERTIES LINKER_LANGUAGE CXX)
