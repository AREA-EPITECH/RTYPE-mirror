file(GLOB_RECURSE SERVER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(r-type_server ${SERVER_SRC_FILES})

target_include_directories(r-type_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    shared
)

# Link dependencies from vcpkg
find_package(spdlog REQUIRED)
target_link_libraries(r-type_server spdlog::spdlog shared)

# Post-build scripts to move binaries to the root directory
if(WIN32)
    set(POST_BUILD_COPY_COMMAND "${CMAKE_COMMAND}" -E copy)
else()
    set(POST_BUILD_COPY_COMMAND "${CMAKE_COMMAND}" -E copy)
endif()

# Define post-build actions for the client
add_custom_command(
    TARGET r-type_server POST_BUILD
    COMMAND ${POST_BUILD_COPY_COMMAND} $<TARGET_FILE:r-type_server> "${CMAKE_SOURCE_DIR}/"
    COMMENT "Moving r-type_server binary to root directory"
)
