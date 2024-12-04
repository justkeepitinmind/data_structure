# CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)
# 项目名称
project(index)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")

include_directories("cppjieba/include")
include_directories("cppjieba/deps/limonp/include")
include_directories("include")

# message(STATUS "CPPJIEBA: ${CPPJIEBA}")
# message(STATUS "INCLUDE : ${INCLUDE}")
set(BUILD_SOURCES
    src/util.cpp
    src/build_index.cpp
    src/index_builder.cpp;
)
message(STATUS "BUILD_SOURCES : ${BUILD_SOURCES}")

add_executable(build_index ${BUILD_SOURCES})

set(SEARCH_SOURCES
    src/search_index.cpp
    src/util.cpp
)

add_executable(search_index ${SEARCH_SOURCES})


set(GET_NEWS_SOURCES
    src/get_news.cpp
    src/util.cpp
)

add_executable(get_news ${GET_NEWS_SOURCES})


set(TEST_SOURCES
    src/performance_test.cpp
    src/util.cpp
)

add_executable(performance_test ${TEST_SOURCES})
