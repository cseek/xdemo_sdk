add_executable(xdemo main.cpp)
target_link_libraries(
    xdemo PRIVATE 
    ${PROJECT_NAME}_static
)
target_include_directories(
    xdemo PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)