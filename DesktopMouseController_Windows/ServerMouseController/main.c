
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#include "error_handling.h"
#include "server.h"
#include "network.h"
#include "main_loop.h"

int main(int argc, char **argv)
{
    SOCKET socket_server;
#ifdef TCP
    SOCKET csocket;
    SOCKADDR_IN cin = { 0 };
#endif

    (void)argc;
    (void)argv;

    if (startup_sockets_mechanism() < 0)
    {
        return EXIT_FAILURE;
    }

    if (run_udp_server(&socket_server) == SERVER_ERROR)
    {
        return EXIT_FAILURE;
    }

#ifdef TCP
    if (server_listen(&socket_server) == FALSE)
    {
        return EXIT_FAILURE;
    }
    csocket = server_accept(&socket_server, &cin);
    if (csocket == INVALID_SOCKET)
    {
        return EXIT_FAILURE;
    }
#endif
    run_main_loop(&socket_server);

    closesocket(socket_server);
#ifdef TCP
    closesocket(csocket);
#endif

    cleanup_sockets_mechanism();
    return EXIT_SUCCESS;
}
