file(GLOB_RECURSE SHARED_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp)

add_library(shared STATIC
    ${SHARED_SRC_FILES}
)

target_include_directories(shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies from vcpkg
find_package(unofficial-enet CONFIG REQUIRED)
target_link_libraries(shared PUBLIC unofficial::enet::enet)
