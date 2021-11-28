cmake_minimum_required(VERSION 3.10)
project(kami)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    SET(BUILD_SHARED_LIBS off)
    add_subdirectory(extern/SFML)
endif()


add_subdirectory(src)
