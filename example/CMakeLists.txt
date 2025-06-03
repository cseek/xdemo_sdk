set(EXE "xdemo")
add_executable(${EXE} main.cpp)
target_link_libraries(
    ${EXE} PRIVATE 
    ${PROJECT_NAME}_static
)
target_include_directories(
    ${EXE} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)