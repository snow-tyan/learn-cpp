#include <cmath>
#include <iostream>
using namespace std;

class Point
{
public:
    Point(int ix = 0, int iy = 0)
        : _ix(ix), _iy(iy)
    {
        cout << "Point(int,int)" << endl;
    }

    void print() const
    {
        cout << "(" << _ix
             << "," << _iy
             << ")" << endl;
    }

protected: //对派生类开放
    int _ix;
    int _iy;
};

class Point3D
    : public Point //接口继承 继承方式会和基类public/protected等做交集
{
public:
    Point3D(int ix = 0, int iy = 0, int iz = 0)
        : Point(ix, iy), _iz(iz)
    {
        cout << "Point3D(int,int,int)" << endl;
    }

    void print() const
    {
        cout << "(" << _ix
             << "," << _iy
             << "," << _iz
             << ")" << endl;
    }

private:
    int _iz;
};

void test0()
{
    Point p2d(1, 2);
    p2d.print();
    Point3D p3d(1, 2, 3);
    p3d.print();
}

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

class A
{
public:
    virtual void a() const { cout << "A::a()" << endl; }
    virtual void b() const { cout << "A::b()" << endl; }
    virtual void c() const { cout << "A::c()" << endl; }
};

class B
{
public:
    virtual void a() const { cout << "B::a()" << endl; }
    virtual void b() const { cout << "B::b()" << endl; }
    void c() const { cout << "B::c()" << endl; } // override
    void d() const { cout << "B::d()" << endl; }
};

class C
    : public A,
      public B
{
public:
    virtual void a() const { cout << "C::a()" << endl; }
    void c() const { cout << "C::c()" << endl; }
    void d() const { cout << "C::d()" << endl; }
};

void test1()
{
    C c;
    A *pA = &c;
    pA->a(); // C::a()
    pA->b(); // A::b()
    pA->c(); // C::c()

    B *pB = &c;
    pB->a(); // C::a()
    pB->b(); // B::b()
    pB->c(); // B::c() c,d非虚函数，只和指针类型有关
    pB->d(); // B::d()

    C *pC = &c;
    pC->a(); // C::a()
    // pC->b();//二义性
    pC->c(); // C::c()
    pC->d(); // C::d()
}

class Base
{
public:
    Base(int base) : _base(base) {}
    virtual void func1() const { cout << "Base::func1()" << endl; }
    virtual void func2() const { cout << "Base::func2" << endl; }

private:
    long _base;
};

class Derived
    : public Base
{
public:
    Derived(long base, long derived) : Base(base), _derived(derived) {}

private:
    long _derived;
};

void test2()
{
    Base base(10);
    cout << "sizeof(Base)=" << sizeof(Base) << endl; //16

    long *p = (long *)&base; 
    cout << p[1] << endl; //10 

    long *p2 = (long *)p[0]; //p[0]是虚函数指针
    // p2[0]; //即p[0][0] 指向第一个入口函数，即Base::func1()
    typedef void (*Function)(void); //Function是函数指针类型，void (*)(void)
    Function f = (Function)p2[0];
    f();
}

int main()
{
    // test0();
    // Rectangle rect(3, 4);
    // Triangle tria(3, 4, 5);
    // print(&rect);
    // print(&tria);

    // test1();
    test2();
    return 0;
}