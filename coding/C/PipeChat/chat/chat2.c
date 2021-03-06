#include "head.h"

int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 3);
    int fdw = open(argv[1], O_WRONLY); //1号管道写端
    int fdr = open(argv[2], O_RDONLY); //2号管道读端
    printf("I am chat2 fdr=%d, fdw=%d\n", fdr, fdw);
    char buf[128] = {0};
    while (1) {
        bzero(buf, sizeof buf);
        read(fdr, buf, sizeof buf);
        printf("gets=%s\n", buf);
        bzero(buf, sizeof buf);
        read(STDIN_FILENO, buf, sizeof buf);
        write(fdw, buf, strlen(buf) - 1); // 减去你输入的\n
    }
    return 0;
}