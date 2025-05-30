file(
    GLOB_RECURSE TEST_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
add_executable(
    test_${PROJECT_NAME}
    ${TEST_SRCS}
)
set_target_properties(
    test_${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)
target_link_libraries(
    test_${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME}_static
    deps::doctest
    deps::spdlog
)
target_include_directories(
    test_${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)