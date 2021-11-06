/**
 * man中用例 
 */
#include <stdint.h> /* Definition of uint64_t */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error(msg)   \
    do {                    \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num>...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int efd = eventfd(10, 0); // 初始值设为10
    if (efd == -1)
        handle_error("eventfd");

    uint64_t u;       // unsigned long
    ssize_t s;        // long
    switch (fork()) { // fork有三种可能，用switch case
    case 0:           // ==0 子进程
        sleep(1);
        for (int i = 1; i < argc; i++) {
            printf("Child writing %s to efd\n", argv[i]);
            u = strtoull(argv[i], NULL, 0); // str to unsignedlonglong
            /* strtoull() allows various bases */
            s = write(efd, &u, sizeof(uint64_t));
            if (s != sizeof(uint64_t))
                handle_error("write");
        }
        printf("Child completed write loop\n");

        exit(EXIT_SUCCESS);
    default: // >0 父进程
        sleep(2);

        printf("Parent about to read\n");
        s = read(efd, &u, sizeof(uint64_t));
        if (s != sizeof(uint64_t))
            handle_error("read");
        printf("Parent read %llu (0x%llx) from efd\n",
               (unsigned long long)u, (unsigned long long)u);
        exit(EXIT_SUCCESS);

    case -1: // -1 出错
        handle_error("fork");
    }
}