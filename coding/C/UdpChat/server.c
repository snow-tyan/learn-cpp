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
    // 一旦bind，端口激活
    int ret = bind(socketfd,
                   (struct sockaddr *)&serverAddr,
                   sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "bind");
    struct sockaddr_in clientAddr;
    bzero(&clientAddr, sizeof clientAddr);
    socklen_t fromLen = sizeof(struct sockaddr);
    char buf[128] = {0};
    recvfrom(socketfd, buf, sizeof buf, 0, (struct sockaddr *)&clientAddr, &fromLen);
    printf("client ip=%s, port=%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    printf("gets=%s\n", buf);
    fd_set rdset;
    while (1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(socketfd, &rdset);
        ret = select(socketfd + 1, &rdset, NULL, NULL, NULL);
        if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &rdset)) {
                bzero(buf, sizeof buf);
                ret = read(STDIN_FILENO, buf, sizeof buf);
                if (0 == ret) {
                    printf("主动断开连接\n");
                    break;
                }
                sendto(socketfd, buf, strlen(buf) - 1, 0,
                       (struct sockaddr *)&clientAddr,
                       sizeof clientAddr);
            }
            if (FD_ISSET(socketfd, &rdset)) {
                bzero(buf, sizeof buf);
                // 不会疯狂打印，无需判断ret==0
                // 因为UDP无连接，server根本不知道client断开
                // TCP之所以知道是因为四次挥手
                recvfrom(socketfd, buf, sizeof buf, 0,
                         (struct sockaddr *)&clientAddr,
                         &fromLen);
                printf("gets=%s\n", buf);
            }
        }
    }
    close(socketfd);
    return 0;
}