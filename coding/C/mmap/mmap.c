#include "head.h"

int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 2); // a.out filename
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    // length 是4k的整数倍，这里设6，其实最小是4k
    char *p = (char *)mmap(NULL, 6, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK(p, (char *)-1, "mmap");
    printf("%s\n", p);
    p[0] = 'H';
    int ret = munmap(p, 6);
    ERROR_CHECK(ret, -1, "munmap");
    close(fd);
    return 0;
}