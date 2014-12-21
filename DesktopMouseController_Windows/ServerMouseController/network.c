
#include <WinSock2.h>
#include "network.h"
#include "error_handling.h"

int startup_sockets_mechanism()
{
    int err;
    WSADATA wsa;

    err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (err < 0)
    {
        print_error(GetLastError());
        return -1;
    }
    return 0;
}

void cleanup_sockets_mechanism()
{
    WSACleanup();
}
