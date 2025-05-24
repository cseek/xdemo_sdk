project(spdlog)
file(
    GLOB_RECURSE ${PROJECT_NAME}_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
add_library(
    ${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_SRCS}
)
add_library(
    deps::${PROJECT_NAME} ALIAS
    ${PROJECT_NAME}
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
    SPDLOG_COMPILED_LIB
)
