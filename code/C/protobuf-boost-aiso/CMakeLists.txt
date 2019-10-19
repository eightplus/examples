project(anbox C CXX)
cmake_minimum_required(VERSION 2.8.2)

include(CTest)
include(GNUInstallDirs)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to RelWithDebInfo")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wno-variadic-macros -Wextra -fPIC")

# By default, all symbols are visible in the library. We strip out things we don't
# want at link time, by running a version script (see unity-scopes.map and the
# setting of LINK_FLAGS below).
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=default")

set(C_AND_CXX_WARNINGS "-pedantic -Wall -Wextra")

# Some additional warnings not included by the general flags set above.
set(EXTRA_C_WARNINGS "-Wcast-align -Wcast-qual -Wformat -Wredundant-decls -Wswitch-default")
set(EXTRA_CXX_WARNINGS "-Wnon-virtual-dtor -Wold-style-cast")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_AND_CXX_WARNINGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_WARNINGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ${C_AND_CXX_WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_WARNINGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_WARNINGS}")

# -fno-permissive causes warnings with clang, so we only enable it for gcc
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-permissive")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Boost COMPONENTS filesystem log serialization system thread program_options)
find_package(PkgConfig)
find_package(Threads)
find_package(Protobuf REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMESA_EGL_NO_X11_HEADERS")

if((Protobuf_VERSION GREATER "3.7") OR (Protobuf_VERSION EQUAL "3.7"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_PROTOBUF_CALLBACK_HEADER")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fPIC")

add_subdirectory(server)
add_subdirectory(client)
