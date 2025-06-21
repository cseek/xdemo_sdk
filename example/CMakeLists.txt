set(EXE_CPP "xdemo")
add_executable(${EXE_CPP} main.cpp)
target_link_libraries(
    ${EXE_CPP} PRIVATE 
    ${PROJECT_NAME}_shared
)
target_include_directories(
    ${EXE_CPP} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)