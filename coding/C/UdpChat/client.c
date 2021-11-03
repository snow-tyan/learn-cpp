#include "head.h"

int main()
{
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0); // udp
    ERROR_CHECK(socketfd, -1, "socket");
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("172.25.40.81");

    char buf[128] = {0};
    strcpy(buf, "I am udp client");
    sendto(socketfd, buf, strlen(buf), 0, (struct sockaddr *)&serverAddr, sizeof serverAddr);

    fd_set rdset;
    while (1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(socketfd, &rdset);
        int ret = select(socketfd + 1, &rdset, NULL, NULL, NULL);
        if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &rdset)) {
                bzero(buf, sizeof buf);
                ret = read(STDIN_FILENO, buf, sizeof buf);
                if (0 == ret) {
                    printf("主动断开连接\n");
                    break;
                }
                sendto(socketfd, buf, strlen(buf) - 1, 0,
                       (struct sockaddr *)&serverAddr,
                       sizeof serverAddr);
            }
            if (FD_ISSET(socketfd, &rdset)) {
                bzero(buf, sizeof buf);
                ret = recvfrom(socketfd, buf, sizeof buf, 0, NULL, NULL);
                if (0 == ret) {
                    printf("对方断开连接\n");
                    break;
                }
                printf("gets=%s\n", buf);
            }
        }
    }
    close(socketfd);
    return 0;
}