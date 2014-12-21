
#include "server.h"
#include "error_handling.h"

SOCKET create_socket()
{
    SOCKET sock = socket(AF_INET,
#ifdef UDP
        SOCK_DGRAM,
#elif defined TCP
        SOCK_STREAM,
#endif
        0);
    if (sock == INVALID_SOCKET)
    {
        print_error(GetLastError());
        return INVALID_SOCKET;
    }
    return sock;
}

SOCKADDR_IN get_socket_configuration()
{
    SOCKADDR_IN sin = { 0 };
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    return sin;
}

int server_bind(SOCKET *socket, const SOCKADDR_IN *sin)
{
    if (bind(*socket, (SOCKADDR *)sin, sizeof(*sin)) == SOCKET_ERROR)
    {
        print_error(GetLastError());
        return SERVER_ERROR;
    }
    return SERVER_OK;
}

int server_listen(SOCKET *socket)
{
    if (listen(*socket, 1) == SOCKET_ERROR)
    {
        print_error(GetLastError());
        return SERVER_ERROR;
    }
    return SERVER_OK;
}

SOCKET server_accept(SOCKET *socket, SOCKADDR_IN *cin)
{
    SOCKET csocket;
    int sinsize = sizeof(*cin);

    csocket = accept(*socket, (SOCKADDR *)cin, &sinsize);
    if (csocket == INVALID_SOCKET)
    {
        print_error(GetLastError());
        return INVALID_SOCKET;
    }
    return csocket;
}

int run_udp_server(SOCKET *socket_server)
{
    SOCKADDR_IN sin;

    *socket_server = create_socket();
    sin = get_socket_configuration();
    if (*socket_server == INVALID_SOCKET)
    {
        return SERVER_ERROR;
    }

    if (server_bind(socket_server, &sin) == SERVER_ERROR)
    {
        closesocket(*socket_server);
        return SERVER_ERROR;
    }
    return SERVER_OK;
}
