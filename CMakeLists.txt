# Set vcpkg toolchain
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

cmake_minimum_required(VERSION 3.14)

if(!WIN32)
    set( CMAKE_CXX_COMPILER "/usr/bin/g++" )
    set( CMAKE_C_COMPILER "/usr/bin/gcc" )
endif()

if(WIN32)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(r-type)

# Enable C++17 or later
set(CMAKE_CXX_STANDARD 17)

# Add subdirectories for client, server, shared code and second game
add_subdirectory(shared)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(tower_defense)