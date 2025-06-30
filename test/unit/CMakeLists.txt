file(
    GLOB_RECURSE TEST_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
add_executable(
    unittest_${PROJECT_NAME}
    ${TEST_SRCS}
)
set_target_properties(
    unittest_${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)
target_link_libraries(
    unittest_${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_static
    deps::doctest
)