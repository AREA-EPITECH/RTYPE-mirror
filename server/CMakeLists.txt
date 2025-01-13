file(GLOB_RECURSE SERVER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(r-type_server ${SERVER_SRC_FILES})

target_include_directories(r-type_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    shared
)
target_link_libraries(r-type_server PRIVATE shared)

# Link dependencies from vcpkg
#find_package(spdlog REQUIRED)
#target_link_libraries(r-type_server PRIVATE spdlog::spdlog)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

set_target_properties(r-type_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/)
