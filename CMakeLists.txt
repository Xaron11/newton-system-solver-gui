cmake_minimum_required(VERSION 3.11)
project(EAN)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

file(GLOB SOURCES
    src/*.cpp
)

file(GLOB HEADERS
    include/*.h
)

add_executable(EAN ${SOURCES} ${HEADERS})
target_link_libraries(EAN gmp mpfr)