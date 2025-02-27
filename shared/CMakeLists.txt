file(GLOB_RECURSE SHARED_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp)

set(RAYGUI_IMPLEMENTATION_FILE "${CMAKE_CURRENT_LIST_DIR}/src/raygui_impl.cpp")

add_library(shared STATIC
    ${SHARED_SRC_FILES}
    ${RAYGUI_IMPLEMENTATION_FILE}
)

target_include_directories(shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies from vcpkg
find_package(fmt CONFIG REQUIRED)
find_package(unofficial-enet CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(shared PUBLIC raylib fmt::fmt spdlog::spdlog unofficial::enet::enet glfw nlohmann_json::nlohmann_json)

# Platform-specific dependencies
if (WIN32)
    add_compile_definitions(SPDLOG_COMPILED_LIB)
    target_link_libraries(shared PUBLIC ws2_32 winmm)
endif()
