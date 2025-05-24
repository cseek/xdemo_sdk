cmake_minimum_required(VERSION 3.10)
project(xdemo_sdk LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(deps/doctest)
add_subdirectory(deps/spdlog)
add_subdirectory(test)

# 打包库
add_custom_target(pack
    COMMAND ${CMAKE_COMMAND} --install .
    COMMAND cd ${CMAKE_INSTALL_PREFIX} && zip -r ${PROJECT_NAME}_${PROJECT_VERSION}.zip ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Creating package"
    DEPENDS ${PROJECT_NAME}_static ${PROJECT_NAME}_shared
)

# 编译示例
add_custom_target(example
    DEPENDS xdemo
    COMMENT "Building example"
)

# 编译示例
add_custom_target(test
    DEPENDS test_${PROJECT_NAME}
    COMMENT "Building unit test"
)