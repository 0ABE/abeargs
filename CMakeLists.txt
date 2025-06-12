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

cmake_minimum_required(VERSION 3.8)
message(STATUS "Using CMake version ${CMAKE_VERSION}")

# ---- Enable C++20 ----------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ---- Stick to the standard -------------------------
set(CMAKE_CXX_EXTENSIONS OFF)
# ----------------------------------------------------

project(AbeArgs VERSION 0.1.250611)

# Define the library target.
add_subdirectory(lib)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    option(BUILD_TESTS "whether or not to build tests" ON)
    if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()

# ---- Choose a build type ---------------------------
#
# NOTE: To set a build type using CMake Tools (VSCode
# extension), comment these lines. If not using CMake
# Tools, uncomment the following to set a build type.
# 
# * * Don't forget the #define for Debug builds. * * *
#
# ---- Debug (with a #define DEBUG_BUILD) ------------
# set(CMAKE_BUILD_TYPE Debug)
# add_compile_definitions(DEBUG_BUILD)
# ---- Release ---------------------------------------
set(CMAKE_BUILD_TYPE Release)
# ----------------------------------------------------
