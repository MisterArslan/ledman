cmake_minimum_required(VERSION 3.5)

project(ledman)

if(NOT(UNIX))
    message(FATAL_ERROR "Only UNIX supported")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message(STATUS "CMAKE_BUILD_TYPE is undefined. Set default build type ${CMAKE_BUILD_TYPE}.")
endif()

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.45.0 REQUIRED COMPONENTS system thread)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

enable_testing()

add_executable(client tests/manual/client.cpp)
add_executable(server tests/manual/server.cpp)

target_link_libraries(client ${Boost_LIBRARIES})
target_link_libraries(server ${Boost_LIBRARIES})

add_test(client client)
add_test(server server)
