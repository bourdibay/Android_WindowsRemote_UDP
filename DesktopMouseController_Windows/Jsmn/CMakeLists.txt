cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()
if(POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif()

Project(Jsmn)

# group source in root directory
file(GLOB sources "*.c" "*.h")

ADD_LIBRARY(Jsmn STATIC ${sources})

TARGET_LINK_LIBRARIES(Jsmn)
