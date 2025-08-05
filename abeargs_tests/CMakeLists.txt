# cmake-format: off
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
# cmake-format: on

# Set the test target name.
set(ABEARGSTESTS_NAME "AbeArgsTests")

# ---- Source code defined  --------------------------
list(APPEND ABEARGSTESTS_SRC_CODE "ParserTests.cpp" "ParserTests.h" "main.cpp")

set(LINUX FALSE)
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(LINUX TRUE)
endif()

set(CLANG_WINDOWS FALSE)
if(CMAKE_SYSTEM_NAME MATCHES "Windows" AND CMAKE_CXX_COMPILER_ID MATCHES
                                           "Clang")
  set(CLANG_WINDOWS TRUE)
endif()

set(GNU_WINDOWS FALSE)
if(CMAKE_SYSTEM_NAME MATCHES "Windows" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(GNU_WINDOWS TRUE)
  if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "MinGW 32-bit Windows build detected.")
    set(CPPUNIT_ARCHIVE_NAME "cppunit-1.15.1-mingw32.zip")
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "UCRT 64-bit Windows build detected.")
    set(CPPUNIT_ARCHIVE_NAME "cppunit-1.15.1-ucrt64.zip")
  endif()
endif()

set(MSVC_WINDOWS FALSE)
if(CMAKE_SYSTEM_NAME MATCHES "Windows" AND CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(MSVC_WINDOWS TRUE)
endif()

# AbeArgsTests is only built in Debug mode, so include the AbeArgs Debug
# libraries.
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  set(CPPUNIT_ARCHIVE
      "${CMAKE_CURRENT_LIST_DIR}/libs/thirdparty/macOS/cppunit-1.15.1.zip")
  set(CPPUNIT_LIBRARY_PATH "lib/Debug/libcppunit.a")
  set(ABEARGS_LIBRARY_PATH "../build/abeargs_lib/libAbeArgsd.a")
endif()

if(LINUX)
  set(CPPUNIT_ARCHIVE
      "${CMAKE_CURRENT_LIST_DIR}/libs/thirdparty/linux/cppunit-1.15.1.tar.gz")
  set(CPPUNIT_LIBRARY_PATH "lib/libcppunit.a")
  set(ABEARGS_LIBRARY_PATH "../build/abeargs_lib/libAbeArgsd.a")
endif()

# cmake-format: off
# if(CLANG_WINDOWS)
#   set(CPPUNIT_ARCHIVE
#       "${CMAKE_CURRENT_LIST_DIR}/libs/thirdparty/win64/cppunit-1.15.1-clang.zip")
#   set(CPPUNIT_LIBRARY_PATH "lib/libcppunit.a")
#   set(ABEARGS_LIBRARY_PATH "../build/abeargs_lib/AbeArgsd.lib")
# endif()
# cmake-format: on

if(GNU_WINDOWS)
  set(CPPUNIT_ARCHIVE
      "${CMAKE_CURRENT_LIST_DIR}/libs/thirdparty/win64/${CPPUNIT_ARCHIVE_NAME}")
  set(CPPUNIT_LIBRARY_PATH "lib/libcppunit.a")
  set(ABEARGS_LIBRARY_PATH "../build/abeargs_lib/libAbeArgsd.a")
endif()

if(MSVC_WINDOWS)
  set(CPPUNIT_ARCHIVE
      "${CMAKE_CURRENT_LIST_DIR}/libs/thirdparty/win64/cppunit-1.15.1-VC.zip")
  set(CPPUNIT_LIBRARY_PATH "lib/cppunitd.lib")
  set(ABEARGS_LIBRARY_PATH "../build/abeargs_lib/Debug/AbeArgsd.lib")
endif()

# Unpack thirdparty libraries.
file(ARCHIVE_EXTRACT INPUT "${CPPUNIT_ARCHIVE}" DESTINATION
     "${CMAKE_CURRENT_LIST_DIR}/build_deps/")
set(CPPUNIT_DIR "${CMAKE_CURRENT_LIST_DIR}/build_deps/cppunit-1.15.1")
set(CPPUNIT_INCLUDE_DIR "${CPPUNIT_DIR}/include")
set(CPPUNIT_LIBRARY "${CPPUNIT_DIR}/${CPPUNIT_LIBRARY_PATH}")
set(ABEARGS_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/${ABEARGS_LIBRARY_PATH}")

# Add source code to this project's executable.
add_executable(${ABEARGSTESTS_NAME} ${ABEARGSTESTS_SRC_CODE})

# Add the include directories for the source code.
target_include_directories(${ABEARGSTESTS_NAME} PRIVATE ${CPPUNIT_INCLUDE_DIR})

# Link the following libraries into the executable. Referencing the AbeArgs
# library here creates a dependency and builds it first.
target_link_libraries(${ABEARGSTESTS_NAME} PRIVATE AbeArgs ${CPPUNIT_LIBRARY})
