#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string>

#define DEFAULT_PORT 5000
static const char LOCALHOST[] = "127.0.0.1";

typedef int SOCK_TYPE;
typedef unsigned int ADDR_SIZE;

void make_sock_addr(
    sockaddr *address,
    ushort port,
    std::string ip_address = ""
);

bool bind_sock_to_addr(SOCK_TYPE &sock, sockaddr *p_address);

bool listen_at_sock(SOCK_TYPE &sock);

bool accept_connection(
    SOCK_TYPE &new_sock,
    SOCK_TYPE &sock,
    sockaddr *sock_addr
);

void get_port_and_ip(sockaddr *saddr, ushort *port, std::string &str);

// bool connect_to_server(SOCK_TYPE &sock, ushort server_port, std::string ip);
