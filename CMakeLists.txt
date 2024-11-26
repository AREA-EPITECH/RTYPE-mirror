# Set vcpkg toolchain
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

cmake_minimum_required(VERSION 3.14)

project(r-type)

# Enable C++17 or later
set(CMAKE_CXX_STANDARD 17)

# Add subdirectories for client, server, and shared code
add_subdirectory(shared)
add_subdirectory(client)
add_subdirectory(server)
