#pragma once

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/socket.h>
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
