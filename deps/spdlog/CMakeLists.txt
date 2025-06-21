project(spdlog)

add_library(
    ${PROJECT_NAME} INTERFACE
    ${${PROJECT_NAME}_SRCS}
)
add_library(
    deps::${PROJECT_NAME} ALIAS
    ${PROJECT_NAME}
)
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# target_compile_definitions(
#     ${PROJECT_NAME} INTERFACE
#     SPDLOG_COMPILED_LIB
# )
