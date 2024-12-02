file(GLOB_RECURSE SERVER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(r-type_server ${SERVER_SRC_FILES})

target_include_directories(r-type_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies from vcpkg
find_package(spdlog REQUIRED)
target_link_libraries(r-type_server spdlog::spdlog)