#include <iostream>
using namespace std;

class MyClass
{
private:
    /* data */
    short &_data;

public:
    MyClass(short data) : _data(data){};
    ~MyClass(){};
    short getData()
    {
        return _data;
    }
};

// void func(int x)
// {
//     cout << x << endl;
// }
// void func(int x, int y = 0)
// {
//     cout << x + y + 10 << endl;
// }

int main()
{
    // int *p = new int; //即使不加() 也是初始化过的
    // int *q = new int[10];
    // cout << *p << endl;
    // cout << *q << *(q + 5) << endl;
    // MyClass *p1 = new MyClass;
    // MyClass *p2 = new MyClass();
    // cout << p1->getData() << endl;
    // cout << p2->getData() << endl;

    MyClass my = MyClass(4);
    MyClass &ref = my;
    cout << sizeof(ref) << endl;  // 8
    cout << sizeof(long) << endl; // 8 64bit

    // func(4);  //二义
    return 0;
}