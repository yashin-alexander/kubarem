set(DUMMY_APP_NAME dummy)
set(EDITOR_APP_NAME editor)

# dummy
add_executable(${DUMMY_APP_NAME} "app.cpp")
target_link_libraries(${DUMMY_APP_NAME} kubarem_lib)
set_property(TARGET ${DUMMY_APP_NAME} PROPERTY CXX_STANDARD 17)
set_target_properties(${DUMMY_APP_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# editor
add_executable(${EDITOR_APP_NAME} "editor.cpp" "editor.h")
target_link_libraries(${EDITOR_APP_NAME} kubarem_lib)
set_property(TARGET ${EDITOR_APP_NAME} PROPERTY CXX_STANDARD 17)
set_target_properties(${EDITOR_APP_NAME} PROPERTIES LINKER_LANGUAGE CXX)
