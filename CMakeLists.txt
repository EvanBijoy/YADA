cmake_minimum_required(VERSION 3.12)
project(YADA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add all source files
file(GLOB SOURCES 
    "src/*.cpp"
)

add_executable(yada ${SOURCES})