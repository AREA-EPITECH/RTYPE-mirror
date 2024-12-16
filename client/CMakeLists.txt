add_executable(r-type_client
    src/main.cpp
)

target_include_directories(r-type_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    shared
)
target_link_libraries(r-type_client PRIVATE shared)

# Link dependencies from vcpkg
#find_package(spdlog REQUIRED)
#target_link_libraries(r-type_server PRIVATE spdlog::spdlog)

set_target_properties(r-type_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/)
