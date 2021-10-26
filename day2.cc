// 1 复习 LRU
#include <string.h>
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;
using PAIR = pair<int, int>;

class LRUCache
{
public:
    LRUCache(int capacity)
        : _capacity(capacity)
    {
    }

    int get(int key)
    {
        auto it = _cache.find(key);
        if (it != _cache.end())
        {
            int ret = it->second->second;
            _list.splice(_list.begin(), _list, it->second);
            _cache[key] = _list.begin();
            return ret;
        }
        return -1;
    }

    void put(int key, int value)
    {
        auto it = _cache.find(key);
        if (it != _cache.end())
        {
            it->second->second = value;
            _list.splice(_list.begin(), _list, it->second);
            _cache[key] = _list.begin();
        }
        else
        {
            if (_list.size() == _capacity)
            {
                int k = _list.back().first;
                _list.pop_back();
                _cache.erase(k);
            }
            _list.push_front(PAIR(key, value));
            _cache[key] = _list.begin();
        }
    }

private:
    int _capacity;
    list<PAIR> _list;
    unordered_map<int, list<PAIR>::iterator> _cache;
};

class Point
{
public:
    explicit Point(int ix = 0, int iy = 0)
        : _ix(ix), _iy(iy)
    {
        cout << "Point(int,int)" << endl;
    }

    ~Point()
    {
        cout << "~Point()" << endl;
    }

    void print()
    {
        cout << "("
             << _ix << "," << _iy
             << ")" << endl;
    }

private:
    int _ix;
    int _iy;
};

class Line
{
private:
    /* data */
    Point _pt1;
    Point _pt2;

public:
    Line(int x1, int y1, int x2, int y2)
        : _pt1(x1, y1), _pt2(x2, y2)
    {
        cout << "Line(int,int,int,int)" << endl;
    }
    ~Line()
    {
        cout << "~Line()" << endl;
    }
    void printLine()
    {
        _pt1.print();
        cout << "-->";
        _pt2.print();
    }
};

class Computer
{
public:
    Computer(const char *brand, int price)
        : _brand(new char[strlen(brand) + 1]()), _price(price)
    {
        cout << "Computer(const char*)" << endl;
    }

    Computer(const Computer &rhs)
        : _brand(new char[strlen(rhs._brand) + 1]()), _price(rhs._price)
    {
        cout << "Computer(const Computer &)" << endl;
        strcpy(_brand, rhs._brand);
    }

    Computer &operator=(const Computer &rhs)
    {
        if (this != &rhs)
        {                    //自复制
            delete[] _brand; //回收左操作数空间
            //深拷贝
            _brand = new char[strlen(rhs._brand) + 1]();
            strcpy(_brand, rhs._brand);
        }
        return *this;
    }

    ~Computer()
    {
        cout << "~Computer()" << endl;
        if (_brand)
            delete[] _brand;
    }

private:
    char *_brand;
    int _price = 0;
    static int _totalSize; //位于全局静态区，必须在类外初始化
};

int Computer::_totalSize = 0;

class Singleton
{
public:
    static Singleton *getInstance()
    {
        if (nullptr == _pInstance)
            return new Singleton();
        return _pInstance;
    }
    static void destory()
    {
        if (_pInstance)
            delete _pInstance;
    }

private:
    Singleton() {}
    ~Singleton() {}

private:
    static Singleton *_pInstance;
};
Singleton *Singleton::_pInstance = nullptr;

// poor String
class MyString
{
public:
    MyString()
        : _pstr(new char())
    {
        cout << "MyString()" << endl;
        strcpy(_pstr, "");
    }
    MyString(const char *pstr)
        : _pstr(new char[strlen(pstr) + 1]())
    {
        cout << "MyString(const char*)" << endl;
        strcpy(_pstr, pstr);
    }
    MyString(const MyString &rhs)
        : _pstr(new char[strlen(rhs._pstr) + 1]())
    {
        cout << "MyString(const MyString &)" << endl;
        strcpy(_pstr, rhs._pstr);
    }
    MyString &operator=(const MyString &rhs)
    {
        cout << "MyString &operator=(const MyString &)" << endl;
        if (this != &rhs) //自复制
        {
            if (_pstr)
                delete[] _pstr; //回收左操作数
            //深拷贝
            _pstr = new char[strlen(rhs._pstr) + 1]();
            strcpy(_pstr, rhs._pstr);
        }
        return *this;
    }
    ~MyString()
    {
        cout << "~MyString()" << endl;
        if (_pstr)
            delete[] _pstr;
    }
    void print() { cout << _pstr << endl; }

private:
    char *_pstr;
};

int main()
{
    // LRUCache cache(2);
    // cache.put(1, 0);
    // cache.put(2, 2);
    // cout << cache.get(1) << endl; // 0
    // cache.put(3, 3);
    // cache.put(1, 1);
    // cout << cache.get(1) << endl; // 1
    // cache.put(4, 4);
    // cout << cache.get(3) << endl; // -1

    // Point pt0; //调用无参构造
    // pt0.print();
    // Point pt1(); //只是声明了一个函数，返回值是Point类型，函数名pt，无参的函数
    //并没有调用构造函数去实例化对象
    // Point pt(1, 2);
    // pt.print();

    // Line line(1, 2, 3, 4);
    // line.printLine();

    // Computer pc("ThinkPad", 8888);
    // cout << sizeof(pc) << endl;
    // Singleton::getInstance();

    MyString str1;
    str1.print();
    MyString str2 = "I am String2";
    str2.print();
    MyString str3("I am String3");
    str3.print();
    // MyString str4(str2);
    MyString str4 = str2;
    str4.print();
    str4 = str3;
    str4.print();
    return 0;
}