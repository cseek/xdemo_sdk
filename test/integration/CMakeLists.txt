file(
    GLOB_RECURSE TEST_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
add_executable(
    integration_test_${PROJECT_NAME}
    ${TEST_SRCS}
)
set_target_properties(
    integration_test_${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)
target_link_libraries(
    integration_test_${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_static
)