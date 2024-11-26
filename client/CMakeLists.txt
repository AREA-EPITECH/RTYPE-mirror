cmake_minimum_required(VERSION 3.29)
project(r-type_client)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    shared
)

# Link dependencies from vcpkg
find_package(spdlog REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog raylib glfw shared)

# Post-build scripts to move binaries to the root directory
if(WIN32)
    set(POST_BUILD_COPY_COMMAND "${CMAKE_COMMAND}" -E copy)
else()
    set(POST_BUILD_COPY_COMMAND "${CMAKE_COMMAND}" -E copy)
endif()

# Define post-build actions for the client
add_custom_command(
    TARGET r-type_client POST_BUILD
    COMMAND ${POST_BUILD_COPY_COMMAND} $<TARGET_FILE:r-type_client> "${CMAKE_SOURCE_DIR}/"
    COMMENT "Moving r-type_client binary to root directory"
)