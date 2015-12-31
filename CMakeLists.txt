cmake_minimum_required(VERSION 3.3)
project(cppkarusellrenn)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    include_directories(/opt/local/include)
    link_libraries(/opt/local/lib/libboost_system-mt.dylib)
    link_libraries(/opt/local/lib/libpqxx.dylib)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")

    include_directories(/usr/local/include)
    link_libraries(/usr/local/lib/libboost_system.so)
endif()

set(SOURCE_FILES main.cpp test.cpp WebsocketServer.cpp WebsocketServer.hpp Database.cpp Database.hpp)
add_executable(cppkarusellrenn ${SOURCE_FILES})