#include "head.h"

int main()
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(socketfd, -1, "socket");
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(8888);
    clientAddr.sin_addr.s_addr = inet_addr("172.25.40.81");

    int ret = connect(socketfd,
                      (struct sockaddr *)&clientAddr,
                      sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1, "connect");
    fd_set rdset;
    char buf[1024] = {0};
    while (1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(socketfd, &rdset);
        do {
            ret = select(socketfd + 1, &rdset, NULL, NULL, NULL);
        } while (ret == -1 && errno == EINTR);
        if (ret == -1) {
            perror("select");
            break;
        } else if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &rdset)) {
                bzero(buf, sizeof buf);
                ret = read(STDIN_FILENO, buf, sizeof buf);
                if (0 == ret) {
                    printf("主动断开连接\n");
                    break;
                }
                ret = send(socketfd, buf, strlen(buf), 0);
                ERROR_CHECK(ret, -1, "send");
            }
            if (FD_ISSET(socketfd, &rdset)) {
                bzero(buf, sizeof buf);
                ret = recv(socketfd, buf, sizeof buf, 0);
                if (0 == ret) {
                    // 防止疯狂打印
                    printf("server断开连接\n");
                    break;
                }
                printf("gets=%s\n", buf);
            }
        }
    }
    close(socketfd);
    return 0;
}