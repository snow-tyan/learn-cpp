#include "head.h"

int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 3);
    int fdw = open(argv[1], O_WRONLY); //1号管道写端
    int fdr = open(argv[2], O_RDONLY); //2号管道读端
    printf("I am chat2 fdr=%d, fdw=%d\n", fdr, fdw);
    char buf[128] = {0};
    fd_set rdset;
    while (1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fdr, &rdset);
        // 只需要监控读事件，并不监控写事件，因此fdr就是”需要监控的最大文件描述符“
        int ret = select(fdr + 1, &rdset, NULL, NULL, NULL);
        if (ret > 0) { //返回文件描述符已改变个数
            if (FD_ISSET(STDIN_FILENO, &rdset)) {
                bzero(buf, sizeof buf);
                ret = read(STDIN_FILENO, buf, sizeof buf);
                // Ctrl+D
                if (0 == ret) {
                    printf("主动断开连接\n");
                    break;
                }
                write(fdw, buf, strlen(buf) - 1); // 减去你输入的\n
            }
            if (FD_ISSET(fdr, &rdset)) {
                bzero(buf, sizeof buf);
                ret = read(fdr, buf, sizeof buf);
                if (0 == ret) { // 退出机制，防止对端退出时疯狂打印
                    printf("对端断开连接\n");
                    break;
                }
                printf("gets=%s\n", buf);
            }
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}