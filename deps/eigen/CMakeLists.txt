project(eigen)

add_library(
    ${PROJECT_NAME} INTERFACE
)
add_library(
    deps::${PROJECT_NAME} ALIAS
    ${PROJECT_NAME}
)
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
