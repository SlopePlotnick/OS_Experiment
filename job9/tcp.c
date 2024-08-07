#include "std.h"
#include "tcp.h"

int tcp_connect(char *ip_addr, int port)
{
    struct sockaddr_in server;
    int fd;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        fatal("socket");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_addr);
    server.sin_port = htons(port);
    if (connect(fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) < 0)
        fatal("bind");
    return fd;
}

int tcp_listen(char *ip_addr, int port)
{
    struct sockaddr_in server;
    int fd;
    int option;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        fatal("socket");

    option = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
        fatal("setsockopt");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip_addr);
    server.sin_port = htons(port);
    if (bind(fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) < 0)
        fatal("bind");
    listen(fd, 8);
    return fd;
}

int tcp_accept(int server_fd)
{
    struct sockaddr_in client;
    socklen_t length = sizeof(struct sockaddr_in);
    int client_fd;

    client_fd = accept(server_fd, (struct sockaddr *)&client, &length);
    if (client_fd < 0)
        fatal("accept");
    return client_fd;
}
