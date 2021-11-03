#include "head.h"

int tcpInit(int *sfd, char *ip, unsigned short port)
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socketfd, -1, "socket");
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);

    int ret = bind(socketfd,
                   (struct sockaddr *)&serverAddr,
                   sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(socketfd, 10);
    *sfd = socketfd;
    return 0;
}

int main()
{
    int socketfd;
    tcpInit(&socketfd, "172.25.40.81", 8888);
    int peerfd = accept(socketfd, NULL, NULL);
    fd_set rdset;
    char buf[1024] = {0};
    while (1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(peerfd, &rdset);
        int ret = select(peerfd + 1, &rdset, NULL, NULL, NULL);
        if (ret > 0) {
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
                    break;
                }
                printf("gets=%s\n", buf);
            }
        }
    }
    close(socketfd);
    close(peerfd);
    return 0;
}