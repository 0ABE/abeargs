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

# ---- Source code defined  --------------------------
list(APPEND ABEARGSTESTS_SRC_CODE
  "ParserTests.cpp"
  "ParserTests.h"
  "main.cpp"
)

# Unpack thirdparty libraries.
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  # CppUnit
  file(ARCHIVE_EXTRACT
       INPUT "${CMAKE_CURRENT_LIST_DIR}/libs/thirdparty/win64/cppunit-1.15.1.zip"
       DESTINATION "${CMAKE_CURRENT_LIST_DIR}/build/_deps/")
  set(CPPUNIT_DIR "${CMAKE_CURRENT_LIST_DIR}/build/_deps/cppunit-1.15.1")
  set(CPPUNIT_LIB_DIR "${CPPUNIT_DIR}/lib")
  set(CPPUNIT_INCLUDE_DIR "${CPPUNIT_DIR}/include")
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CPPUNIT_LIBRARY "${CPPUNIT_LIB_DIR}/cppunit.lib")
  else()
    set(CPPUNIT_LIBRARY "${CPPUNIT_LIB_DIR}/cppunitd.lib")
  endif()
endif()

# Path to the AbeArgs library.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(ABEARGS_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/../build/lib/Release")
else()
  set(ABEARGS_LIB_DIR "${CMAKE_CURRENT_LIST_DIR}/../build/lib/Debug")
endif()

# Add source code to this project's executable.
add_executable(AbeArgsTests ${ABEARGSTESTS_SRC_CODE})


target_include_directories(AbeArgsTests PRIVATE
                            ${CPPUNIT_INCLUDE_DIR})

# Link the following libraries into the executable.
target_link_libraries(AbeArgsTests PRIVATE
                      AbeArgs ${CPPUNIT_LIBRARY})
