cmake_minimum_required(VERSION 2.8.2)

#set(CMAKE_C_FLAGS "-Wall")
#set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set (CMAKE_C_FLAGS "-g -O0 -fno-stack-protector ${CMAKE_C_FLAGS} ${HARDENED_C_FLAGS}")


# Ensure -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(server)
add_subdirectory(client)
