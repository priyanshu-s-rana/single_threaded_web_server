cmake_minimum_required(VERSION 3.10)


# set(CMAKE_CXX_COMPILER /opt/homebrew/bin/aarch64-apple-darwin24-c++-14)
project(WebServer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES 
    "src/*.cpp"
    "Networking/*.cpp"
    "utils/*.cpp"
)

add_executable(WebServer ${SOURCES})

target_include_directories(WebServer PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Networking    
)