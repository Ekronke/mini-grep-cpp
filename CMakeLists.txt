cmake_minimum_required(VERSION 3.10)
project(MyParserProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(mini-grep src/main.cpp src/lexer.cpp src/parser.cpp)

target_include_directories(mini-grep PRIVATE include)

target_compile_options(mini-grep PRIVATE -Wall -Wextra -Werror)
