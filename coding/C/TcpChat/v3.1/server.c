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

int setNonblock(int fd)
{
    int status = fcntl(fd, F_GETFL); // 获取文件状态
    status |= O_NONBLOCK;            // 按位或上 非阻塞
    int ret = fcntl(fd, F_SETFL, status);
    ERROR_CHECK(ret, -1, "fcntl");
    return 0;
}

int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 1);
    int socketfd;
    int peerfd;
    int ret = tcpInit(&socketfd, "172.25.40.81", 8888);
    if (-1 == ret) {
        return -1;
    }
    int epfd = epoll_create1(0);
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET; //读事件 或上 EPOLLET 边缘触发 提高效率
    event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    event.data.fd = socketfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, socketfd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    char buf[8] = {0}; // buf设小，体会Nonblock
    struct epoll_event *evs = (struct epoll_event *)calloc(MAXCLIENT, sizeof(struct epoll_event));
    while (1) {
        int readyn = epoll_wait(epfd, evs, MAXCLIENT, -1);
        for (int i = 0; i < readyn; ++i) {
            if (evs[i].events & EPOLLIN && evs[i].data.fd == socketfd) {
                peerfd = accept(socketfd, NULL, NULL);
                ERROR_CHECK(peerfd, -1, "accept");
                ret = setNonblock(peerfd); // 设置非阻塞
                if (-1 == ret) {
                    return -1;
                }
                event.data.fd = peerfd; // 注册peerfd
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, peerfd, &event);
                ERROR_CHECK(ret, -1, "epoll_ctl");
                printf("a client connected\n");
            }
            if (evs[i].events & EPOLLIN && evs[i].data.fd == STDIN_FILENO) {
                bzero(buf, sizeof buf);
                ret = read(STDIN_FILENO, buf, sizeof buf);
                if (0 == ret) {
                    printf("server broken up\n");
                    goto end;
                }
                send(peerfd, buf, strlen(buf) - 1, 0);
            }
            if (evs[i].events & EPOLLIN && evs[i].data.fd == peerfd) {
                while (1) {
                    bzero(buf, sizeof buf);
                    ret = recv(peerfd, buf, sizeof buf, 0);
                    if (0 == ret) {
                        // 防止疯狂打印
                        printf("a client disconnected\n");
                        // 将peerfd从event中解除注册
                        ret = epoll_ctl(epfd, EPOLL_CTL_DEL, peerfd, &event);
                        ERROR_CHECK(ret, -1, "epoll_ctl");
                        close(peerfd);
                        break; //跳出for循环，还在外层while(1)中
                    } else if (-1 == ret) {
                        //非阻塞，缓冲区空时
                        break;
                    } else {
                        printf("%s", buf);
                    }
                }
                printf("\n");
            }
        }
    }
end:
    free(evs);
    close(socketfd);
    close(epfd);
    return 0;
}