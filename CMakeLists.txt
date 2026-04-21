# ==================================================
# Project Setup
# ==================================================
cmake_minimum_required(VERSION 3.20)
project(Ping-Pung VERSION 0.1.0 LANGUAGES C CXX)

# ==================================================
# Build Settings
# ==================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==================================================
# Source & Header Collection
# ==================================================
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
set(ALL_SOURCES ${SOURCES} ${HEADERS})

# ==================================================
# Executable Target & Includes
# ==================================================
add_executable(${PROJECT_NAME} ${ALL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# ==================================================
# vcpkg Packages
# ==================================================
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib glfw)

# ==================================================
# Output / Tools
# ==================================================
if (CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(
        copy_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
                "${CMAKE_BINARY_DIR}/compile_commands.json"
                "${CMAKE_SOURCE_DIR}/compile_commands.json"
        DEPENDS "${CMAKE_BINARY_DIR}/compile_commands.json"
        COMMENT "Creating symlink for compile_commands.json in source dir"
    )
endif()