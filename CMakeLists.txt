cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O3 -Wall -Wextra -Wpedantic)


file(GLOB CPP_FILES Source/*.cpp)
add_executable(nSearch main.cpp ${CPP_FILES})

include_directories(nSearch PRIVATE Include)
target_include_directories(nSearch PRIVATE external/aixlog/include)

add_subdirectory(external/CPPurses)
target_link_libraries(nSearch cppurses)
