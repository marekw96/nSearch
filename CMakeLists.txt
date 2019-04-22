cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
add_executable(nSearch main.cpp)
target_include_directories(nSearch PRIVATE Include)

add_subdirectory(external/CPPurses)
target_link_libraries(nSearch cppurses)
