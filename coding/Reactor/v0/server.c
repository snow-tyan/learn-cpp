// very simple tcp server

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define ARGS_CHECK(argc, val)        \
    {                                \
        if (argc != val) {           \
            printf("args error!\n"); \
            return -1;               \
        }                            \
    }
#define ERROR_CHECK(ret, val, funcName) \
    {                                   \
        if (val == ret) {               \
            perror(funcName);           \
            return -1;                  \
        }                               \
    }

// #define MAXEVENTS 2

int main(int argc, char *argv[])
{
    // socket -> bind -> listen
    int socketFd = socket(AF_INET, SOCK_STREAM, 0); // ipv4, tcp
    ERROR_CHECK(socketFd, -1, "socket");
    printf("hi");
    // 设置地址、端口可重用
    int on = 1;
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
        perror("setsockopt reuse addr");
        close(socketFd);
        return -1;
    }
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on))) {
        perror("setsockopt reuse port");
        close(socketFd);
        return -1;
    }

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons((short)atoi("8888"));       // port 主机字节序转网络字节序
    serverAddr.sin_addr.s_addr = inet_addr("172.25.40.81"); // ip 点分十进制转网络字节序

    int ret = bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(socketFd, 10);
    ERROR_CHECK(ret, -1, "listen");

    while (1) {
        int clientFd = accept(socketFd, NULL, NULL);
        char buff[1024] = "Welcome to server.";
        ret = write(clientFd, buff, strlen(buff));
        memset(buff, 0, sizeof(buff));
        printf(">> pls client data:\n");
        ret = read(clientFd, buff, sizeof(buff));
        printf("%s", buff);
    }
    close(socketFd);

    return 0;
}