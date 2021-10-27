// 工厂模式

#include <math.h>
#include <iostream>
#include <memory>

using namespace std;

// #define SimpleFactory
#define FactoryMethod

class Figure
{
public:
    virtual void print() const = 0;
    virtual double area() = 0;
    virtual ~Figure() { cout << "~Figure()" << endl; }
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
        cout << "This is a rectangle.";
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
        cout << "This is a triangle.";
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
    cout << " 其面积是 " << figure->area() << endl;
}

// 静态工厂方法
// 缺点：1 不满足单一职责原则：一个类最多只做一件事，一个引起它变化的原因
//      2 不满足开放闭合原则：有新的图像加进来时，需要修改Factory类
#ifdef SimpleFactory
class Factory
{
public:
    static Rectangle createRectangle()
    {
        // load from file...
        Rectangle rectangle(3, 4);
        return rectangle;
    }

    static Triangle createTriangle()
    {
        Triangle triangle(3, 4, 5);
        return triangle;
    }
};

void test()
{ // 简单工厂模式
    Rectangle rect = Factory::createRectangle();
    Triangle tria = Factory::createTriangle();
    print(&rect);
    print(&tria);
}
#endif

// 工厂方法

#ifdef FactoryMethod
class Factory
{
public:
    virtual Figure *create() = 0;
    virtual ~Factory() { cout << "~Factory()" << endl; }
};

class RectangleFactory
    : public Factory
{
public:
    Figure *create()
    {
        // load from file
        return new Rectangle(3, 4);
    }
};
class TriangleFactory
    : public Factory
{
public:
    Figure *create()
    {
        // load from file...
        return new Triangle(3, 4, 5);
    }
};

void test()
{
    unique_ptr<Factory> rectFactory(new RectangleFactory());
    unique_ptr<Figure> rectangle(rectFactory->create());
    unique_ptr<Factory> triaFactory(new TriangleFactory());
    unique_ptr<Figure> triangle(triaFactory->create());
    print(rectangle.get());
    print(triangle.get());
}
#endif

int main()
{
    test();
    return 0;
}