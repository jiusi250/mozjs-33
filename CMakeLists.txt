cmake_minimum_required(VERSION 3.10)
project(decompiler)

set(CMAKE_CXX_STANDARD 11)
set(SPIDER_BUILD_DIR ${PROJECT_SOURCE_DIR}/js/src/build_debug)

include_directories(${SPIDER_BUILD_DIR}/dist/include)
link_directories(${SPIDER_BUILD_DIR}/dist/lib)

add_executable(decompiler main.cpp)

target_link_libraries(decompiler js_static z)
