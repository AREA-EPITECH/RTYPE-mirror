add_library(shared STATIC
    src/NetworkWrapper.cpp
    src/Packet.cpp
)

target_include_directories(shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies from vcpkg
find_package(unofficial-enet CONFIG REQUIRED)
target_link_libraries(shared PUBLIC unofficial::enet::enet)
