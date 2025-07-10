# 
#           d8888 888                     d8888                          
#          d88888 888                    d88888                          
#         d88P888 888                   d88P888                          
#        d88P 888 88888b.   .d88b.     d88P 888 888d888 .d88b.  .d8888b  
#       d88P  888 888 "88b d8P  Y8b   d88P  888 888P"  d88P"88b 88K      
#      d88P   888 888  888 88888888  d88P   888 888    888  888 "Y8888b. 
#     d8888888888 888 d88P Y8b.     d8888888888 888    Y88b 888      X88 
#    d88P     888 88888P"   "Y8888 d88P     888 888     "Y88888  88888P' 
#                                                           888          
# ~$ Command Line Argument Processing Simplified       Y8b d88P          
#                                                       "Y88P"           
# Copyright (c) 2025, Abe Mishler
# Licensed under the Universal Permissive License v 1.0
# as shown at https://oss.oracle.com/licenses/upl/.              
# 

cmake_minimum_required(VERSION 3.10)
message(STATUS "Using CMake version ${CMAKE_VERSION}")

project("AbeArgs" VERSION 0.1.250709)

# ---- Enable C++20 ----------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ---- Stick to the standard -------------------------
set(CMAKE_CXX_EXTENSIONS OFF)
# ----------------------------------------------------

# Choose the build type.
# Default to Release if not specified.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type: Release" FORCE)
# set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type: Debug" FORCE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")
    add_compile_definitions(DEBUG_BUILD)
    # Build tests only if the build type is Debug.
    add_subdirectory(tests)
else()
    message(STATUS "Building in Release mode")
    add_compile_definitions(RELEASE_BUILD)
endif()

# Define the AbeArgs library target.
add_subdirectory(lib)
