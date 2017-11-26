cmake_minimum_required(VERSION 3.8)
project(plasma_i3_tool)

set(CMAKE_C_STANDARD 99)

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

set(SOURCE_FILES main.c)
add_executable(plasma_i3_tool ${SOURCE_FILES})

install(TARGETS plasma_i3_tool RUNTIME DESTINATION bin)
