cmake_minimum_required(VERSION 3.11)
project(EAN)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_BUILD_TYPE Release)

string(REPLACE "-mno-direct-extern-access" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB_RECURSE SOURCES
    src/*.cpp
)
file(GLOB_RECURSE HEADERS
    include/*.h
)
file(GLOB_RECURSE UI_FILES "ui/*.ui")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

include_directories(${Qt6_INCLUDE_DIRS})
include_directories(${Qt6Widgets_INCLUDE_DIRS})

qt_add_executable(EAN MANUAL_FINALIZATION  ${SOURCES} ${HEADERS} ${UI_FILES})

target_link_libraries(EAN PRIVATE gmp mpfr Qt6::Widgets)

qt_finalize_executable(EAN)