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
list(APPEND ABEARGS_SRC_CODE
    "Argument.cpp"
    "Argument.h"
    "Defaults.h"
    "Math.h"
    "Parser.cpp"
    "Parser.h"
    "Util.h"
)

if(MSVC)
list(APPEND ABEARGS_SRC_CODE
    "MSVC.h"
)
endif()

# ---- Shared or static libraries --------------------
# add_library(AbeArgs SHARED ${ABEARGS_SRC_CODE})
add_library(AbeArgs STATIC ${ABEARGS_SRC_CODE})
# ----------------------------------------------------

# Name the library target based on the build type.
set_target_properties(AbeArgs PROPERTIES
    OUTPUT_NAME_DEBUG "AbeArgsd"
    OUTPUT_NAME_RELEASE "AbeArgs"
)

# Export the include dir.
set(ABEARGS_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "AbeArgs include dir")
