set(EXE_CPP "xdemo")
add_executable(${EXE_CPP} main.cpp)
target_link_libraries(
    ${EXE_CPP} PRIVATE 
    ${PROJECT_NAME}_static
)