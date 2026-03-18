cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_COMPILER clang)

project(raylib_proj LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)



# Define raylib location
set(RAYLIB_DIR ${CMAKE_BINARY_DIR}/raylib)
set(RAYLIB_LIB ${RAYLIB_DIR}/lib/libraylib.so)
set(RAYLIB_INCLUDE_DIR ${RAYLIB_DIR}/include)

# Verify raylib exists
if(NOT EXISTS ${RAYLIB_LIB})
    message(FATAL_ERROR "raylib not found at: ${RAYLIB_LIB}")
endif()



# Default to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Debug: sanitizers + no optimisation
set(CMAKE_C_FLAGS_DEBUG   "-g -O0 -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "-fsanitize=address -fsanitize=undefined")

# Release: full optimisation
set(CMAKE_C_FLAGS_RELEASE   "-O3 -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")



# source files list
set(SRC_FILES
    src/entity.c
)

# main exe 
add_executable(main 
    src/ray_main.c
    ${SRC_FILES}
)

# Include raylib headers, plus ours
target_include_directories(main PRIVATE 
    ${RAYLIB_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link raylib
target_link_libraries(main PRIVATE ${RAYLIB_LIB})



