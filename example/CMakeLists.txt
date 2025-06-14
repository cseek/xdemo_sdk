# c语言例子
set(EXE_C "xdemoc")
add_executable(${EXE_C} main.c)
target_link_libraries(
    ${EXE_C} PRIVATE 
    ${PROJECT_NAME}_shared
)
target_include_directories(
    ${EXE_C} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# c++ 例子
set(EXE_CPP "xdemocpp")
add_executable(${EXE_CPP} main.cpp)
target_link_libraries(
    ${EXE_CPP} PRIVATE 
    ${PROJECT_NAME}_shared
)
target_include_directories(
    ${EXE_CPP} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)