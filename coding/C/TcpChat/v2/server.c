#include "head.h"

#define MAXCLIENT 10

// socket+bind+listen
int tcpInit(int *sfd, char *ip, unsigned short port)
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socketfd, -1, "socket");
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    //bind之前，设置可重用
    int reuseAddr = 1;
    int reusePort = 1;
    int ret = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(int));
    ERROR_CHECK(ret, -1, "setsockopt");
    ret = setsockopt(socketfd, SOL_SOCKET, SO_REUSEPORT, &reusePort, sizeof(int));
    ERROR_CHECK(ret, -1, "setsockopt");
    ret = bind(socketfd,
               (struct sockaddr *)&serverAddr,
               sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(socketfd, 10);
    *sfd = socketfd;
    return 0;
}

int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 1);
    int socketfd;
    int ret = tcpInit(&socketfd, "172.25.40.81", 8888);
    if (-1 == ret) {
        return -1;
    }
    int peerfd;
    fd_set rdset;
    fd_set needMoniterSet; // 始终记录需要监控的描述符，不传给select
    FD_ZERO(&needMoniterSet);
    FD_SET(STDIN_FILENO, &needMoniterSet);
    FD_SET(socketfd, &needMoniterSet);
    char buf[1024] = {0};
    while (1) {
        memcpy(&rdset, &needMoniterSet, sizeof(fd_set));
        // 0 1 2被用了，假设最大10个client端连接，就是13+1
        ret = select(MAXCLIENT + 3 + 1, &rdset, NULL, NULL, NULL);
        if (ret > 0) {
            if (FD_ISSET(socketfd, &rdset)) {
                peerfd = accept(socketfd, NULL, NULL);
                ERROR_CHECK(peerfd, -1, "accept");
                FD_SET(peerfd, &needMoniterSet);
            }
            if (FD_ISSET(STDIN_FILENO, &rdset)) {
                bzero(buf, sizeof buf);
                ret = read(STDIN_FILENO, buf, sizeof buf);
                if (0 == ret) {
                    printf("主动断开连接\n");
                    break;
                }
                send(peerfd, buf, strlen(buf) - 1, 0);
            }
            if (FD_ISSET(peerfd, &rdset)) {
                bzero(buf, sizeof buf);
                ret = recv(peerfd, buf, sizeof buf, 0);
                if (0 == ret) {
                    // 防止疯狂打印
                    printf("对端断开连接\n");
                    // 将peerfd从needMoniterSet移除
                    FD_CLR(peerfd, &needMoniterSet);
                    close(peerfd);
                } else {
                    printf("gets=%s\n", buf);
                }
            }
        }
    }
    close(socketfd);
    close(peerfd);
    return 0;
}