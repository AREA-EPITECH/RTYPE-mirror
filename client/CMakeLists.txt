add_executable(r-type_client
    src/main.cpp
)

target_include_directories(r-type_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies from vcpkg
find_package(spdlog REQUIRED)
target_link_libraries(r-type_client PRIVATE spdlog::spdlog)