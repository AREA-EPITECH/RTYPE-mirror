add_executable(r-type_server
    src/main.cpp
)

target_include_directories(r-type_server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    shared
)

# Link dependencies from vcpkg
find_package(spdlog REQUIRED)
target_link_libraries(r-type_server spdlog::spdlog shared)
