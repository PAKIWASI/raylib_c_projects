cmake_minimum_required(VERSION 3.20)
project(raylib_proj LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Define raylib location
set(RAYLIB_DIR ${CMAKE_BINARY_DIR}/raylib)
set(RAYLIB_LIB ${RAYLIB_DIR}/lib/libraylib.so)
set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/include)

# Verify raylib exists
if(NOT EXISTS ${RAYLIB_LIB})
    message(FATAL_ERROR "raylib not found at: ${RAYLIB_LIB}")
endif()



file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")

# add source files
add_executable(main ${SRC_FILES})

# Include raylib headers (and yours)
target_include_directories(main PRIVATE 
    ${RAYLIB_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link raylib
target_link_libraries(main PRIVATE ${RAYLIB_LIB} m)
