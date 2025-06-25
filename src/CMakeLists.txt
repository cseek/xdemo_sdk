file(
    GLOB_RECURSE SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
add_library(
    ${PROJECT_NAME}_static STATIC
    ${SRCS}
)
add_library(
    ${PROJECT_NAME}_shared SHARED
    ${SRCS}
)
target_link_libraries(
    ${PROJECT_NAME}_static PRIVATE
    deps::spdlog
    deps::json
    deps::eigen
)
target_link_libraries(
    ${PROJECT_NAME}_shared PRIVATE
    deps::spdlog
    deps::json
    deps::eigen
)
# 添加头文件包含路径
target_include_directories(
    ${PROJECT_NAME}_static PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_include_directories(
    ${PROJECT_NAME}_shared PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
# 设置输出名称
set_target_properties(
    ${PROJECT_NAME}_static PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)
set_target_properties(
    ${PROJECT_NAME}_shared PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    C_VISIBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)
target_compile_options(
    ${PROJECT_NAME}_static PRIVATE
    $<$<CONFIG:DEBUG>: -O0 -Wall -g>
    $<$<CONFIG:RELEASE>: -O2>
)
# 使用`-fvisibility=hidden`来默认隐藏所有符号，再通过`visibility("default")`显式导出需要的符号。
# 这样可以有效减少动态库的符号冲突和优化加载性能。
target_compile_options(
    ${PROJECT_NAME}_shared PRIVATE
    $<$<CONFIG:DEBUG>: -O0 -Wall -g -fvisibility=hidden>
    $<$<CONFIG:RELEASE>: -O2 -DNDEBUG -fvisibility=hidden>
)
target_compile_definitions(
    ${PROJECT_NAME}_shared PRIVATE
    XDEMO_SDK_SHARED
    GIT_HASH="${GIT_HASH}"
)

target_compile_definitions(
    ${PROJECT_NAME}_static PRIVATE
    GIT_HASH="${GIT_HASH}"
)

# 安装目标
install(
    TARGETS ${PROJECT_NAME}_static ${PROJECT_NAME}_shared
    ARCHIVE DESTINATION ${PROJECT_NAME}/lib
    LIBRARY DESTINATION ${PROJECT_NAME}/lib
    RUNTIME DESTINATION ${PROJECT_NAME}/bin
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/xdemo_sdk.h
    DESTINATION ${PROJECT_NAME}/include
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    DESTINATION ${PROJECT_NAME}/include
)
install(
    FILES ${TOP}/example/main.cpp
    DESTINATION ${PROJECT_NAME}/example
)