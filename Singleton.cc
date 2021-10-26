/*
五种单例模式
*/

#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
using namespace std;

// #define easylazy
// #define neibulei
// #define raii
// #define bestlazy
#define ehan

#ifdef easylazy
class Singleton
{
public:
    static Singleton *getInstance()
    {
        if (nullptr == _pInstance)
            _pInstance = new Singleton();
        return _pInstance;
    }
    ~Singleton() { cout << "~Sinleton()" << endl; } //没有打印，不会执行，有内存泄漏

private:
    Singleton() { cout << "Singleton()" << endl; }

private:
    static Singleton *_pInstance;
};
Singleton *Singleton::_pInstance;

#endif

#ifdef neibulei
// Singleton 懒汉 内部类
class Singleton
{
public:
    static Singleton *getInstance()
    {
        if (nullptr == _pInstance)
        { //双重检测锁，只有判空才加锁，节省开销
            lock_guard<mutex> lock(_mutex);
            if (nullptr == _pInstance)
                _pInstance = new Singleton();
        }
        return _pInstance;
    }

private:
    Singleton() { cout << "Singleton()" << endl; };
    ~Singleton() { cout << "~Singleton()" << endl; }
    class Grabo
    {
    public:
        ~Grabo()
        {
            cout << "~Grabo()" << endl;
            if (Singleton::_pInstance)
                delete Singleton::_pInstance;
            Singleton::_pInstance = nullptr;
        }
    };
    static Grabo gra;

private:
    static Singleton *_pInstance;
    static mutex _mutex;
};
Singleton *Singleton::_pInstance;
mutex Singleton::_mutex;
Singleton::Grabo Singleton::gra = Grabo();
#endif

#ifdef raii
class Singleton
{
public:
    using Ptr = shared_ptr<Singleton>;
    static Ptr getInstance()
    {
        if (nullptr == _pInstance)
        {
            lock_guard<mutex> lock(_mutex);
            if (nullptr == _pInstance)
                _pInstance = Ptr(new Singleton());
        }
        return _pInstance;
    }
    ~Singleton() { cout << "~Singleton()" << endl; } //智能指针，析构函数不能私有化

private:
    Singleton() { cout << "Singleton()" << endl; }

private:
    static Ptr _pInstance;
    static mutex _mutex;
};
Singleton::Ptr Singleton::_pInstance;
mutex Singleton::_mutex;

#endif

#ifdef bestlazy
class Singleton
{
public:
    static Singleton &getInstance()
    {
        static Singleton instance;
        return instance;
    }

private:
    Singleton() { cout << "Singleton()" << endl; }
    ~Singleton() { cout << "~Singleton()" << endl; }
};
#endif

#ifdef ehan
class Singleton
{
public:
    static Singleton &getInstance()
    {
        return _instance;
    }
private:
    Singleton(){cout<<"Singleton()"<<endl;}
    ~Singleton(){cout<<"~Singleton()"<<endl;}
private:
    static Singleton _instance;
};
Singleton Singleton::_instance;  //main函数开始之前就实例化
#endif


int main()
{
    cout << "MainFunc Start" << endl;
#ifdef easylazy
    Singleton *p1 = Singleton::getInstance();
    Singleton *p2 = Singleton::getInstance();
#endif
#ifdef neibulei
    Singleton *p1 = Singleton::getInstance();
    Singleton *p2 = Singleton::getInstance();
#endif
#ifdef raii
    Singleton::Ptr p1 = Singleton::getInstance();
    Singleton::Ptr p2 = Singleton::getInstance();
#endif
#ifdef bestlazy
    Singleton &p1 = Singleton::getInstance();
    Singleton &p2 = Singleton::getInstance();
#endif
#ifdef ehan
    Singleton &p1 = Singleton::getInstance();
    Singleton &p2 = Singleton::getInstance();
#endif
    // cout << p1 << endl
    //      << p2 << endl;

    cout << &p1 << endl
         << &p2 << endl;
    cout << "MainFunc Done" << endl;
    return 0;
}