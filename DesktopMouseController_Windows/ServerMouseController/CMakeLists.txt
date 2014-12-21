cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif()

project(ServerMouseController)

# only way to add headers in generated solution
file(GLOB headers "*.h")
file(GLOB sources 
"error_handling.c"
"json_msg_parser.c"
"main_loop.c"
"mouse_controller.c"
"network.c"
"server.c"
"main.c")

INCLUDE_DIRECTORIES (
    ./
)

ADD_EXECUTABLE( ServerMouseController
    ${sources}
    ${headers}
)

TARGET_LINK_LIBRARIES(ServerMouseController Jsmn Ws2_32)
ADD_DEPENDENCIES( ServerMouseController Jsmn )
