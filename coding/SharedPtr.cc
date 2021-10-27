// simple shared_ptr

#include <iostream>
#include <memory>
using namespace std;

template <class T>
class MySharedPtr
{
public:
    MySharedPtr(T *p = nullptr)
        : _p(p), _refCnt(new int())
    {
        // _refCnt已经重开内存了++和=1没区别
        ++(*_refCnt); // *refCnt = 1
        cout << "MySharedPtr()" << endl;
    }
    MySharedPtr(const MySharedPtr<T> &msp)
        : _p(msp._p), _refCnt(msp._refCnt)
    {
        ++(*msp._refCnt);
        cout << "MySharedPtr(const MySharedPtr &)" << endl;
    }
    ~MySharedPtr() { release(); }
    MySharedPtr &operator=(const MySharedPtr<T> &msp)
    {
        if (this != &msp)
        {
            release();
            _p = msp._p;
            _refCnt = msp._refCnt;
            ++(*_refCnt);
        }
        cout << "MySharedPtr &operator=(const MySharedPtr &)" << endl;
        return *this;
    }

    T *get() const { return _p; }
    T *operator->() const { return _p; }
    T &operator*() const { return *_p; }
    int use_count() const { return (*_refCnt); }

    void reset(T *p)
    {
        release();
        _p = p;
        _refCnt = new int();
        ++(*_refCnt);
    }

private:
    void release()
    {
        --(*_refCnt);
        if ((*_refCnt) == 0)
        {
            delete _p;
            _p = nullptr;
            delete _refCnt;
            _refCnt = nullptr;
            cout << ">> release heap" << endl;
        }
    }

private:
    T *_p;
    int *_refCnt;
};

class Point
    : public std::enable_shared_from_this<Point>
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

struct Foo
{
    Foo(int n = 0) noexcept : bar(n)
    {
        std::cout << "Foo: constructor, bar = " << bar << '\n';
    }
    ~Foo()
    {
        std::cout << "Foo: destructor, bar = " << bar << '\n';
    }
    int getBar() const noexcept { return bar; }

private:
    int bar;
};

void test0()
{
    // shared_ptr<Point> sp(new Point(1, 1));
    // sp->print();
    // Point *pt1 = new Point(2, 2);
    // Point *pt2 = new Point(3, 3);
    // shared_ptr<Point> sp2(pt1);
    // shared_ptr<Point> sp3(pt1); //一个裸指针交给两个智能指针托管，double free
    // // shared_ptr<Point> sp3(sp2);
    // cout << sp3.use_count() << endl;

    shared_ptr<Foo> sptr(new Foo(1));
    cout << "The first Foo's bar is " << sptr->getBar() << endl;

    // 重置，交与新的 Foo 实例
    // （此调用后将销毁旧实例）
    sptr.reset(new Foo);
    cout << "The second Foo's bar is " << sptr->getBar() << endl;
}

void test1()
{
    // MySharedPtr<Point> sp(new Point(1, 1));
    // sp->print();
    // Point *pt1 = new Point(2, 2);
    // Point *pt2 = new Point(3, 3);
    // MySharedPtr<Point> sp2(pt1);
    // MySharedPtr<Point> sp3(pt1); //一个裸指针交给两个智能指针托管，double free
    // // MySharedPtr<Point> sp3(sp2);
    // cout << sp3.use_count() << endl;
    // MySharedPtr<Point> sp4(pt2);
    // cout << pt2 << endl;
    // cout << sp4.get() << endl;
    // cout << *sp4 << endl;
    MySharedPtr<Foo> sptr(new Foo(1));
    cout << "The first Foo's bar is " << sptr->getBar() << endl;

    // 重置，交与新的 Foo 实例
    // （此调用后将销毁旧实例）
    sptr.reset(new Foo);
    cout << "The second Foo's bar is " << sptr->getBar() << endl;
}

int main()
{
    test0();
    cout << "---------------------------" << endl;
    test1();
    return 0;
}