// TLS 线程局部存储

#include <stdio.h>
#include <error.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
using namespace std;

int g = 1; // 全局变量

__thread int g2 = 1; // 线程局部存储
// 每个线程独有一个errno和g2
void *threadFunc1(void *arg)
{
    printf("\nSubThread 1:\n");
    printf("&g = %p\n", &g);
    printf("g = %d\n", g);
    printf("&errno = %p\n", &errno);
    printf("errno = %d\n", errno);
    printf("&g2 = %p\n", &g2);
    printf("g2 = %d\n", g2);

    return nullptr;
}

void *threadFunc2(void *arg)
{
    printf("\nSubThread 2:\n");
    printf("&g = %p\n", &g);
    printf("g = %d\n", g);
    printf("&errno = %p\n", &errno);
    printf("errno = %d\n", errno);
    printf("&g2 = %p\n", &g2);
    printf("g2 = %d\n", g2);
    return nullptr;
}

void test0() // 创建两个线程再join errno和g2的地址不同
{
    printf("MainThread:\n");
    printf("&g = %p\n", &g);
    printf("g = %d\n", g);
    printf("&errno = %p\n", &errno);
    printf("errno = %d\n", errno);
    printf("&g2 = %p\n", &g2);
    printf("g2 = %d\n", g2);
    pthread_t pthid1, pthid2;
    pthread_create(&pthid1, nullptr, threadFunc1, nullptr);
    pthread_create(&pthid2, nullptr, threadFunc2, nullptr);
    ::usleep(1000);
    pthread_join(pthid1, nullptr);
    pthread_join(pthid2, nullptr);
}
void test1() // 创建一个线程join一个 errno和g2的地址相同
{
    printf("MainThread:\n");
    printf("&g = %p\n", &g);
    printf("g = %d\n", g);
    printf("&errno = %p\n", &errno);
    printf("errno = %d\n", errno);
    printf("&g2 = %p\n", &g2);
    printf("g2 = %d\n", g2);
    pthread_t pthid1, pthid2;
    pthread_create(&pthid1, nullptr, threadFunc1, nullptr);
    pthread_join(pthid1, nullptr);
    ::usleep(1000);

    pthread_create(&pthid2, nullptr, threadFunc2, nullptr);
    pthread_join(pthid2, nullptr);
}

int main()
{
    test0();
    cout << endl;
    test1();

    return 0;
}