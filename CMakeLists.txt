cmake_minimum_required(VERSION 3.10)
project(decompiler)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 11)
set(SPIDER_BUILD_DIR ${PROJECT_SOURCE_DIR}/js/src/build_debug)

file(MAKE_DIRECTORY ${SPIDER_BUILD_DIR})

ExternalProject_Add(spidermonkey
        PREFIX ${SPIDER_BUILD_DIR}/dist
        DOWNLOAD_COMMAND ""
        SOURCE_DIR ${SPIDER_BUILD_DIR}
        CONFIGURE_COMMAND ${SPIDER_BUILD_DIR}/../configure_osx.sh
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND "")

add_library(js_static STATIC IMPORTED)
set_property(TARGET js_static PROPERTY IMPORTED_LOCATION ${SPIDER_BUILD_DIR}/dist/lib/libjs_static.a)

include_directories(${SPIDER_BUILD_DIR}/dist/include)

add_executable(decompiler main.cpp)
add_dependencies(decompiler spidermonkey)
target_link_libraries(decompiler js_static z)
