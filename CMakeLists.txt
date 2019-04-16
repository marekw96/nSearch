cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
add_executable(nSearch main.cpp)

add_subdirectory(external/CPPurses)
target_link_libraries(nSearch cppurses)
