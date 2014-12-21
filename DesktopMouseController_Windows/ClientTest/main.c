
#include <WinSock2.h>
#include <Windows.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "../ServerMouseController/error_handling.h"

#define PORT 10500
#define PORT_STR "10500"
#define IP_ADRESS "127.0.0.1"

void send_data(SOCKET *socket, SOCKADDR_IN *to, char const *msg)
{
    char buffer[1024] = { 0 };
    int tosize = sizeof(*to);
    strcpy_s(buffer, 1024, msg);
    if (sendto(*socket, buffer, (int)strlen(buffer), 0, (SOCKADDR *)to, tosize) < 0)
    {
        print_error(GetLastError());
    }
}

void build_msg(const int x, const int y, char *buffer, const size_t length)
{
    static int prev_x = 0;
    static int prev_y = 0;
    memset(buffer, '\0', length);
    sprintf_s(buffer, length, "{\"from_x\" : %d, \"from_y\" : %d, \"to_x\" : %d, \"to_y\" : %d }", prev_x, prev_y, x, y);
    prev_x = x;
    prev_y = y;
}

int main(int argc, char **argv)
{
    WSADATA wsa;

    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (err < 0)
    {
        print_error(GetLastError());
        return FALSE;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        print_error(GetLastError());
        return EXIT_FAILURE;
    }

    const char *hostname = IP_ADRESS;
    struct addrinfo aiHints;
    struct addrinfo *aiList = NULL;

    // Setup the hints address info structure
    // which is passed to the getaddrinfo() function
    memset(&aiHints, 0, sizeof(aiHints));
    aiHints.ai_family = AF_INET;
    aiHints.ai_socktype = SOCK_DGRAM;
    aiHints.ai_protocol = IPPROTO_UDP;

    struct addrinfo *res;
    getaddrinfo(IP_ADRESS, PORT_STR, &aiHints, &aiList);
    struct sockaddr_in* saddr = NULL;
    for (res = aiList; res != NULL; res = res->ai_next)
    {
        /* ideally look at the sa_family here to make sure it is AF_INET before casting */
        saddr = (struct sockaddr_in*)res->ai_addr;

        //        printf("hostname: %s\n", inet_ntoa(saddr->sin_addr));
    }

    SOCKADDR_IN to = { 0 };
    to.sin_addr = saddr->sin_addr;
    to.sin_port = htons(PORT);
    to.sin_family = AF_INET;

    char buffer[1024] = { 0 };
    int x, y;
    x = 14;
    y = 74;
    build_msg(x, y, buffer, sizeof(buffer));
    send_data(&sock, &to, buffer);
    printf("Wait for input\n");
    char c = ' ';
    do
    {
        printf("Get ch\n");
        c = _getch();
        printf("Get ch ok\n");
        switch (c) {
        case 'w':
            --y;
            break;
        case 's':
            ++y;
            break;
        case 'd':
            ++x;
            break;
        case 'a':
            --x;
            break;
        }

        printf("Message ready to be sent\n");
        build_msg(x, y, buffer, sizeof(buffer));
        send_data(&sock, &to, buffer);
    } while (c != 'q');


    closesocket(sock);
    WSACleanup();
    return 0;
}
