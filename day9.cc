// 面向对象可以实现多态
// 基于对象也可以！

#include <math.h>
#include <iostream>
#include <functional>

using namespace std;
#define BO

// 面向对象 Object Oriented
// 面向对象使用纯虚函数实现多态
#ifndef BO
class Figure
{
public:
    virtual void print() const = 0;
    virtual double area() = 0;
};

class Rectangle
    : public Figure
{
public:
    Rectangle(int length, int width)
        : _length(length), _width(width) { cout << "Rectangle(int,int)" << endl; }
    ~Rectangle() { cout << "~Rectangle()" << endl; }
    void print() const
    {
        cout << "This is a rectangle." << endl;
    }

    double area()
    {
        return _length * _width;
    }

private:
    int _length;
    int _width;
};

class Triangle
    : public Figure
{
public:
    Triangle(int a, int b, int c)
        : _a(a), _b(b), _c(c)
    {
        cout << "Triangle(int,int,int)" << endl;
    }
    ~Triangle() { cout << "~Triangle()" << endl; }
    void print() const
    {
        cout << "This is a triangle." << endl;
    }
    double area()
    {
        double p = (_a + _b + _c) / 2;
        return sqrt(p * (p - _a) * (p - _b) * (p - _c));
    }

private:
    int _a;
    int _b;
    int _c;
};

void print(Figure *figure)
{
    figure->print();
    cout << "其面积是 " << figure->area() << endl;
}

void test0()
{
    Rectangle *rect = new Rectangle(3, 4);
    Triangle tria(3, 4, 5);
    print(rect);
    print(&tria);
    delete rect;
}
#endif

// 基于对象 Object Based
// 基于对象使用回调函数实现多态

#ifdef BO
class Figure
{
public:
    using DisplayCallback = std::function<void()>;
    using AreaCallback = std::function<double()>;

    //注册回调函数
    void setDisplayCallback(const DisplayCallback &cb)
    {
        _displayCb = cb;
    }
    void setAreaCallback(const AreaCallback &cb)
    {
        _areaCb = cb;
    }

    //执行回调函数
    void handleDisplayCallback()
    {
        if (_displayCb)
            _displayCb();
    }
    double handleAreaCallback()
    {
        if (_areaCb)
            return _areaCb();
        return 0.;
    }

private:
    DisplayCallback _displayCb;
    AreaCallback _areaCb;
};

// 如果来个新类，无需继承，方法也无需同名，参数数量也无限制
// 只需返回值类型匹配
class Rectangle
{
public:
    Rectangle(int length, int width)
        : _length(length), _width(width)
    {
        // cout << "Rectangle(int,int)" << endl;
    }
    // ~Rectangle() { cout << "~Rectangle()" << endl; }
    void print() const
    {
        cout << "This is a rectangle." << endl;
    }

    double area()
    {
        return _length * _width;
    }

private:
    int _length;
    int _width;
};

class Triangle
{
public:
    Triangle(int a, int b, int c)
        : _a(a), _b(b), _c(c)
    {
        // cout << "Triangle(int,int,int)" << endl;
    }
    // ~Triangle() { cout << "~Triangle()" << endl; }
    void display() const
    {
        cout << "This is a triangle." << endl;
    }
    double mianji()
    {
        double p = (_a + _b + _c) / 2;
        return sqrt(p * (p - _a) * (p - _b) * (p - _c));
    }

private:
    int _a = 0;
    int _b = 0;
    int _c = 0;
};

void print(Figure &figure) // Figure现在是具体类
{
    figure.handleDisplayCallback();
    cout << "其面积是 " << figure.handleAreaCallback() << endl;
}

void test1()
{
    Figure figure;
    Rectangle rect(3, 4);
    Triangle tria(3, 4, 5);

    figure.setDisplayCallback(std::bind(&Rectangle::print, &rect));
    figure.setAreaCallback(std::bind(&Rectangle::area, &rect));
    print(figure);

    // 传对象和传地址的区别
    // 传对象 + bind又是值传递，会析构很多次，暂不知缘由
    figure.setDisplayCallback(std::bind(&Triangle::display, tria));
    figure.setAreaCallback(std::bind(&Triangle::mianji, tria));
    print(figure);
}
#endif

int main()
{
    // test0();
    test1();
    return 0;
}