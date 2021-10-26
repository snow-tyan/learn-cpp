// Template Singleton
#include <string.h>
#include <iostream>
using namespace std;
#define POINTLAZY

#ifdef POINTLAZY
template <typename T>
class Singleton
{
    class AutoRelease
    {
    public:
        AutoRelease() { cout << "AutoRelease()" << endl; }
        ~AutoRelease()
        {
            if (_pInstance)
                delete _pInstance;
            cout << "~AutoRelease()" << endl;
        }
    };

public:
    template <typename... Args>
    static T *getInstance(Args... args)
    {
        if (nullptr == _pInstance)
        {
            // ::atexit(destory);
            _auto;  // 必须显示调用一下auto，模板只有调用时才会进行推导
            _pInstance = new T(args...);
        }
        return _pInstance;
    }
    // static void destory()
    // {
    //     if (_pInstance)
    //         delete _pInstance;
    // }

private:
    Singleton() { cout << "Singleton()" << endl; }
    ~Singleton() { cout << "~Singleton()" << endl; }

private:
    static T *_pInstance;
    static AutoRelease _auto;
};
template <typename T>
T *Singleton<T>::_pInstance; //初始化
template <typename T>
class Singleton<T>::AutoRelease Singleton<T>::_auto; //显示告诉编译器AutoRelease是一个类型 class/typename

#endif
#ifndef POINTLAZY
template <typename T>
class Singleton
{
public:
    template <typename... Args>
    static T &getInstance(Args... args)
    {
        static T instance = T(args...);
        return instance;
    }

private:
    Singleton() { cout << "Singleton()" << endl; }
    ~Singleton() { cout << "~Singleton()" << endl; }
};
#endif
class Point
{
public:
    Point(int ix = 0, int iy = 0)
        : _ix(ix), _iy(iy)
    {
        cout << "Point(int,int)" << endl;
    }
    ~Point() { cout << "~Point(int,int)" << endl; }

private:
    int _ix;
    int _iy;
};

class Computer
{
public:
    Computer(const char *brand, double price)
        : _brand(new char[strlen(brand) + 1]()), _price(price)
    {
        cout << "Computer(char*, int)" << endl;
    }
    //拷贝构造 赋值运算符略
    ~Computer()
    {
        cout << "~Computer()" << endl;
        if (_brand)
            delete[] _brand;
    }

private:
    char *_brand;
    double _price;
};
#ifdef POINTLAZY
void test0()
{
    Point *p1 = Singleton<Point>::getInstance(1, 2);
    Point *p2 = Singleton<Point>::getInstance(1, 2);
    Point *p3 = Singleton<Point>::getInstance(1, 3);

    cout << "p1'address=" << p1 << endl;
    cout << "p2'address=" << p2 << endl;
    cout << "p3'address=" << p3 << endl;

    // Computer *p4 = Singleton<Computer>::getInstance("xiaomi", 6666);
    // Computer *p5 = Singleton<Computer>::getInstance("xiaomi", 6666);
    // Computer *p6 = Singleton<Computer>::getInstance("dami", 7777);

    // cout << "p4'address=" << p4 << endl;
    // cout << "p5'address=" << p5 << endl;
    // cout << "p6'address=" << p6 << endl;
}
#endif
#ifndef POINTLAZY
void test1()
{
    Point &p1 = Singleton<Point>::getInstance(1, 2);
    Point &p2 = Singleton<Point>::getInstance(1, 2);
    Point &p3 = Singleton<Point>::getInstance(1, 3);

    cout << "p1'address=" << &p1 << endl;
    cout << "p2'address=" << &p2 << endl;
    cout << "p3'address=" << &p3 << endl;

    Computer &p4 = Singleton<Computer>::getInstance("xiaomi", 6666);
    Computer &p5 = Singleton<Computer>::getInstance("xiaomi", 6666);
    Computer &p6 = Singleton<Computer>::getInstance("dami", 7777);

    cout << "p4'address=" << &p4 << endl;
    cout << "p5'address=" << &p5 << endl;
    cout << "p6'address=" << &p6 << endl;
}
#endif
int main()
{
    test0();
    // test1();
    return 0;
}