#include "helpers.h"
#include <iostream>


void make_sock_addr(
    sockaddr *address,
    ushort port,
    std::string ip_address
) {
    sockaddr_in *sock_addr = (sockaddr_in*)address;
    sock_addr->sin_family = AF_INET;
    sock_addr->sin_port = htons(port);
    if (ip_address == "") {
        sock_addr->sin_addr.s_addr = INADDR_ANY;
    } else {
        sock_addr->sin_addr.s_addr = inet_addr(ip_address.c_str());
    }
}

bool bind_sock_to_addr(SOCK_TYPE &sock, sockaddr *p_address) {
    if (bind(sock, p_address, sizeof(*p_address)) < 0) {
        std::cout << "bind() failed" << std::endl;
        return false;
    }
    return true;
}

bool listen_at_sock(SOCK_TYPE &sock) {
    if (listen(sock, 1) < 0) {
        std::cout << "listen() failed" << std::endl;
        return false;
    }
    return true;
}

bool accept_connection(
    SOCK_TYPE &new_sock,
    SOCK_TYPE &sock,
    sockaddr *sock_addr
) {
    ADDR_SIZE addr_size = sizeof(*sock_addr);
    new_sock = accept(sock, sock_addr, (socklen_t*)&addr_size);
    if (new_sock == -1) {
        std::cout << "accept() failed" << std::endl;
        return false;
    }
    return true;
}

void get_port_and_ip(sockaddr *saddr, ushort *port, std::string &str) {
    sockaddr_in * client_addr = (sockaddr_in *) saddr;
    
    *port = ntohs(client_addr->sin_port);
   
    char ip_buffer[40];
    inet_ntop(AF_INET, (void *)&(client_addr->sin_addr), ip_buffer, 40);
    std::cout << ip_buffer << std::endl;
    str = std::string(ip_buffer);
}



bool connect_to_server(SOCK_TYPE &sock, ushort server_port, std::string ip){
 
    sockaddr server_addr;
    make_sock_addr(&server_addr, server_port, ip);
    ADDR_SIZE socklen = sizeof(server_addr);
    if(connect(sock, &server_addr, socklen) < 0){
        std::cout << "connect() failed" << std::endl;
        return false;
    }
    return true;
}


