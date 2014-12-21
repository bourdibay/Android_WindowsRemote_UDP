
#ifndef __SERVER_H__
#define __SERVER_H__

#include <WinSock2.h>

#define PORT 10500

/*
#define TCP
*/
#define UDP

#define SERVER_ERROR -1
#define SERVER_OK 0

int run_udp_server(SOCKET *socket_server);


SOCKET create_socket();
SOCKADDR_IN get_socket_configuration();
int server_bind(SOCKET *socket, const SOCKADDR_IN *sin);
int server_listen(SOCKET *socket);
SOCKET server_accept(SOCKET *socket, SOCKADDR_IN *cin);

#endif
