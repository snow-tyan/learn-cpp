#include "head.h"

int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 3);
    int fdr = open(argv[1], O_RDONLY); //1号管道读端
    int fdw = open(argv[2], O_WRONLY); //2号管道写端
    printf("I am chat1 fdr=%d, fdw=%d\n", fdr, fdw);
    char buf[128] = {0};
    fd_set rdset;
    while (1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fdr, &rdset);
        // 只需要监控读事件，并不监控写事件，因此fdr就是”需要监控的最大文件描述符“
        int ret = select(fdr + 1, &rdset, NULL, NULL, NULL);
        if (ret > 0) { //返回文件描述符已改变个数，有描述符就绪ret>0, TIMEOUT设置NULL永久等待，ret>0这个条件可以不写
            if (FD_ISSET(STDIN_FILENO, &rdset)) { //监控标准输入
                bzero(buf, sizeof buf);
                ret = read(STDIN_FILENO, buf, sizeof buf);
                // Ctrl+D
                if (0 == ret) {
                    printf("主动断开连接\n");
                    break;
                }
                write(fdw, buf, strlen(buf) - 1); // 减去你输入的\n
            }
            if (FD_ISSET(fdr, &rdset)) { //监控管道读端
                bzero(buf, sizeof buf);
                // 写端关闭，读端会返回0，fdr状态会被标识为可读
                ret = read(fdr, buf, sizeof buf);
                if (0 == ret) { // 退出机制，防止对端退出时疯狂打印
                    printf("对端断开连接!\n");
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