cmake_minimum_required(VERSION 2.8)
project (muriel)

set (PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SRC ${PROJECT_SOURCE_DIR}/Url.cpp ${PROJECT_SOURCE_DIR}/ByteRange.cpp)

include_directories(${PROJECT_INCLUDE_DIR})
add_library(${PROJECT_NAME} STATIC ${SRC})

