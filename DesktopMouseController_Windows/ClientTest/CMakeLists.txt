cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif()

project(ClientTest)

file(GLOB sources 
"../ServerMouseController/error_handling.c"
"../ServerMouseController/json_msg_parser.c"
"../ServerMouseController/main_loop.c"
"../ServerMouseController/mouse_controller.c"
"../ServerMouseController/network.c"
"../ServerMouseController/server.c"
"main.c")
file(GLOB headers "*.h" ../ServerMouseController/*.h)

INCLUDE_DIRECTORIES (
    ./
)

ADD_EXECUTABLE( ClientTest
    ${sources}
    ${headers}
)

TARGET_LINK_LIBRARIES(ClientTest Jsmn Ws2_32)
ADD_DEPENDENCIES( ClientTest Jsmn )
