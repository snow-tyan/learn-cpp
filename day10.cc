#include <iostream>
#include <memory>
#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>
using namespace std;
// using namespace boost;

template <class T>
class RAII
{
public:
    RAII(T *p) : _p(p) { cout << "RAII()" << endl; }
    ~RAII()
    {
        if (_p)
            delete _p;
        cout << "~RAII()" << endl;
    }
    T *operator->() { return _p; }
    T &operator*() { return *_p; }
    T *get() { return _p; }
    void reset(T *p)
    {
        if (_p)
            delete _p;
        _p = p;
    }

private:
    T *_p;
};

class Point
:public std::enable_shared_from_this<Point>
{
public:
    Point(int ix = 0, int iy = 0) : _ix(ix), _iy(iy) { cout << "Point(int,int)" << endl; }
    ~Point() { cout << "~Point()" << endl; }
    void print() const
    {
        cout << "(" << _ix << "," << _iy << ")" << endl;
    }
    friend ostream &operator<<(ostream &os, const Point &pt);
    shared_ptr<Point> addPoint(Point *pt)
    {
        _ix += pt->_ix;
        _iy += pt->_iy;
        return shared_from_this();
    }

private:
    int _ix;
    int _iy;
};
ostream &operator<<(ostream &os, const Point &pt)
{
    return os << "(" << pt._ix << "," << pt._iy << ")";
}

void test0()
{
    RAII<Point> pointer(new Point(1, 2));
    cout << *pointer << endl;
    pointer->print();
}
#if 0
void test1() // auto_ptr C++17已删除
{
    Point *pt = new Point(1, 2);
    auto_ptr<Point> ap(pt);
    ap->print();

    auto_ptr<Point> ap2(ap); //调用拷贝构造
    // 但是拷贝构造底层，执行的是转移操作 ap2拿到ap托管的对象，ap的this指针置空
    ap2->print();
    // ap->print(); //段错误
}
#endif
unique_ptr<Point> getPoint()
{
    unique_ptr<Point> up(new Point(2, 1));
    return up; //调用移动构造
}
void test2() // unique_ptr
{
    Point *pt = new Point(1, 2);
    unique_ptr<Point> up(pt);
    up->print();
    cout << up.get() << endl; // 获取原生指针

    // unique_ptr<Point> up2(up); //拷贝构造=delete
    unique_ptr<Point> up2(std::move(up));
    // vector<Point*> points; // 存的是原生裸指针，只有new没有delete
    // boost::ptr_vector<Point> points;  // 1. boost库指针容器 ptr_vector
    // points.push_back(new Point(1, 2));
    // points.push_back(new Point(1, 3));
    // points.push_back(new Point(1, 4));
    // vector<unique_ptr<Point>> points;                     // 2. 使用unique_ptr
    // points.push_back(unique_ptr<Point>(new Point(1, 2))); // 匿名对象，移动构造
    // points.push_back(unique_ptr<Point>(new Point(1, 2)));
    // // points.push_back(up); // 传递左值，会调用拷贝构造，但是unique_ptr没有拷贝构造
    // points.push_back(std::move(up)); // 具有移动语义

    // up = getPoint(); // 移动语义
}

void test3() // shared_ptr
{
    shared_ptr<Point> sp(new Point(1, 2));
    (*sp).print();
    sp->print();

    shared_ptr<Point> sp2(sp);
    sp2->print();
    sp->print();
    cout << sp.use_count() << endl; //引用计数
}

class Child;
class Parent
{
public:
    Parent() { cout << "Parent()" << endl; }
    ~Parent() { cout << "~Parent()" << endl; }

    shared_ptr<Child> _pChild;
};

class Child
{
public:
    Child() { cout << "Child()" << endl; }
    ~Child() { cout << "~Child()" << endl; }

    // shared_ptr<Parent> _pParent;
    weak_ptr<Parent> _pParent;
};

void test4() // shared_ptr 循环引用
{
    shared_ptr<Child> pChild(new Child());
    shared_ptr<Parent> pParent(new Parent());
    cout << pChild.use_count() << endl;  // 1
    cout << pParent.use_count() << endl; // 1

    pParent->_pChild = pChild;           // 引用计数+1
    pChild->_pParent = pParent;          // 引用计数+1
    cout << pChild.use_count() << endl;  // 2
    cout << pParent.use_count() << endl; // 2
}

void test5() // weak_ptr
{
    shared_ptr<Child> pChild(new Child());
    shared_ptr<Parent> pParent(new Parent());
    cout << pChild.use_count() << endl;  // 1
    cout << pParent.use_count() << endl; // 1

    pParent->_pChild = pChild;           // 引用计数+1
    pChild->_pParent = pParent;          // weak_ptr 引用计数不变
    cout << pChild.use_count() << endl;  // 2
    cout << pParent.use_count() << endl; // 1
}

void test6()
{
    Point *pt1 = new Point(1, 2);
    Point *pt2 = new Point(2, 1);
    shared_ptr<Point> sp1(pt1);
    shared_ptr<Point> sp2(pt2);

    shared_ptr<Point> sp3(sp1->addPoint(sp2.get())); // correct
}
int main()
{
    // test0();
    // test1();
    // test2();
    // test3();
    // test4();
    // test5();
    test6();
    return 0;
}