#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define ARGS_CHECK(argc, val)        \
    {                                \
        if (argc != val) {           \
            printf("error args!\n"); \
            return -1;               \
        }                            \
    }

#define ERROR_CHECK(ret, val, funcName) \
    {                                   \
        if (ret == val) {               \
            perror(funcName);           \
            return -1;                  \
        }                               \
    }
