#include <math.h>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;
using namespace std::placeholders;

void print()
{
    cout << "print()" << endl;
}
void display()
{
    cout << "display()" << endl;
}
void test0() // C风格实现多态，使用函数指针
{
    // void(*f)(void) = print;
    typedef void (*Function)(void);
    Function f = print;
    f();
    f = display;
    f();
}

void test1() // C++风格，使用std::function
{
    std::function<void()> f = print;
    f();
    f = display;
    f();
}

class Example
{
public:
    Example() = default;
    void print()
    {
        cout << "Example::print()" << endl;
    }

public:
    int _data = 10;
};

void test2() // 那要如何指向类成员函数呢？类成员函数有个this指针。
             // std::function + std::bind
{
    std::function<void()> f;
    Example e;
    // bind1st bind2nd C++17已弃用
    // bind(T&& f, Args&&... args)
    // f (Callable) 函数对象、函数指针、函数引用、指向成员函数指针、指向数据成员指针
    // 返回类型是函数对象
    // 绑定this指针，可以是一个对象e/对象取地址&e，如果传地址，需保证对象生命周期还存在
    f = std::bind(&Example::print, e);
    f();
    auto g = std::bind(&Example::_data, e); // 指向不同的东西，返回值类型也不同
    cout << "Example::_data = " << g() << endl;
}

int add(int x, int y, int z)
{
    cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
    return x + y + z;
}

int addRef(int &x, int &y, const int &z)
{
    cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
    return x + y + z;
}

void test3()
{
    auto f = std::bind(add, _2, _1, 30);
    cout << f(1, 2) << endl;

    auto f2 = std::bind(add, 10, _1, 30);
    cout << f2(1, 2, 3) << endl; // 多余参数无效，只接受第一个参数

    // bind是值传递，默认会拷贝非占位符对象，如传引用，需用std::ref 引用包装器
    // ref(变量)
    // 占位符 序号 表示实参传递顺序
    int a = 10; 
    const int c = 30;
    int b = 20;
    auto f3 = std::bind(addRef, std::ref(a), _1, std::cref(c));
    cout << f3(std::ref(b)) << endl;
}

class Number
{
public:
    Number(int data) : _data(data) {}

    void display() const { cout << _data << " "; }
    bool isEven() const { return _data % 2 == 0; }
    bool isPrime() const
    {
        if (_data <= 1)
            return false;
        for (int i = 2; i <= sqrt(_data); ++i)
        {
            if (_data % i == 0)
                return false;
        }
        return true;
    }
    bool isNotPrime() const { return !isPrime(); }

private:
    int _data;
};

void test4() // mem_fn
{
    vector<Number> numbers;
    for (int i = 1; i <= 30; ++i)
    {
        // numbers.push_back(Number(i)); //调用构造
        numbers.emplace_back(i);
    }
    // for (auto it = numbers.begin();it!=numbers.end();++it){
    //     it->display();
    // }
    // for_each(InputIt first, InputIt last, unaryFunction f)
    // for_each(numbers.begin(), numbers.end(), bind(&Number::display, _1)); // 一元谓词
    for_each(numbers.begin(), numbers.end(), mem_fn(&Number::display));
    cout << endl;
    numbers.erase(std::remove_if(numbers.begin(), numbers.end(), mem_fn(&Number::isEven)), numbers.end());
    for_each(numbers.begin(), numbers.end(), mem_fn(&Number::display));
    cout << endl;
    numbers.erase(std::remove_if(numbers.begin(), numbers.end(), mem_fn(&Number::isNotPrime)), numbers.end());
    for_each(numbers.begin(), numbers.end(), mem_fn(&Number::display));
    cout << endl;
}

template <class T>
void f1(T &t)
{
    cout << "left value" << endl;
}
template <class T>
void f1(T &&t)
{
    cout << "right value" << endl;
}

template <class T>
void NotPerfectForward(T &&t)
{
    cout << "NotPerfectForward" << endl;
    f1(t);
}
template <class T>
void PerfectForward(T &&t)
{
    cout << "PerfectForward" << endl;
    f1(std::forward<T>(t));
}

void test5() // perfect forwarding
{
    int a = 3;
    NotPerfectForward(a); //left value
    NotPerfectForward(std::move(a)); //left value

    PerfectForward(a); //left value
    PerfectForward(std::move(a)); // right value
}

int main()
{
    // test0();
    // test1();
    // test2();
    // test3();
    // test4();
    test5();
    return 0;
}