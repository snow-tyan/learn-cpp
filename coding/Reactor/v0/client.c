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

int main(int argc, char *argv[])
{
    // socket - connect
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socketFd, -1, "socket");

    struct sockaddr_in clientAddr;
    bzero(&clientAddr, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons((short)atoi("8888"));       // port 主机字节序转网络字节序
    clientAddr.sin_addr.s_addr = inet_addr("172.25.40.81"); // ip 点分十进制转网络字节序

    int ret = connect(socketFd, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
    ERROR_CHECK(ret, -1, "connect");

    char buff[1024] = {0};
    read(socketFd, buff, sizeof(buff));
    printf("%s\n", buff);

    char recvbuff[1024] = {0};
    char sendbuff[1024] = {0};
    while (1) {
        fgets(sendbuff, sizeof sendbuff, stdin);
        write(socketFd, sendbuff, strlen(sendbuff));

        int ret = read(socketFd, recvbuff, sizeof(recvbuff));
        if (-1 == ret) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        } else if (0 == ret) {
            printf("server closed!\n");
            close(socketFd);
            return -1;
        }
        printf("recv msg from server: %s\n", recvbuff);
        bzero(recvbuff, sizeof(recvbuff));
        bzero(sendbuff, sizeof sendbuff);
    }
    return 0;
}