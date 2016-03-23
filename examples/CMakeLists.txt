include_directories(
    ${PROJECT_SOURCE_DIR}/include)

add_executable(example example.cpp)

set_target_properties(example PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON)

add_custom_command(TARGET example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/cmudict_SPHINX_40.txt $<TARGET_FILE_DIR:example>)
