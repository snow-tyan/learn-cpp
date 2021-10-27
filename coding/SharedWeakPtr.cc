// simple shared_ptr and weak_ptr

#include <iostream>
#include <memory>

using namespace std;

struct Counter
{
    Counter() = default;
    int _sharedCount = 0; // shared_ptr 引用计数
    int _weakCount = 0;   // weak_ptr 引用计数
};

template <class T>
class MyWeakPtr;

template <class T>
class MySharedPtr
{
public:
    MySharedPtr(T *p = nullptr)
        : _p(p), _refCnt(new Counter())
    {
        if (p)
            _refCnt->_sharedCount = 1;
        cout << "MySharedPtr()" << endl;
    }
    MySharedPtr(const MySharedPtr<T> &msp)
        : _p(msp._p)
    {
        ++msp._refCnt->_sharedCount;
        _refCnt = msp._refCnt;
        cout << "MySharedPtr(const MySharedPtr &)" << endl;
    }
    MySharedPtr(const MyWeakPtr<T> &mwp)
        : _p(mwp._p)
    {
        ++mwp._refCnt->_sharedCount;
        _refCnt = mwp._refCnt;
        cout << "MySharedPtr(const MyWeakPtr &)" << endl;
    }
    ~MySharedPtr()
    {
        release();
        cout << "~MySharedPtr()" << endl;
    }
    MySharedPtr<T> &operator=(const MySharedPtr<T> &msp)
    {
        if (this != &msp)
        {
            this->release();
            _p = msp._p;
            ++msp._refCnt->_sharedCount;
            _refCnt = msp._refCnt;
            cout << "MySharedPtr &operator=(const MySharedPtr &)" << endl;
        }
        return *this;
    }

    T &operator*() const { return *_p; }
    T *operator->() const { return _p; }
    T *get() const { return _p; }
    int use_count() const { return _refCnt->_sharedCount; }

    friend class MyWeakPtr<T>;

private:
    void release()
    {
        --_refCnt->_sharedCount;
        if (_refCnt->_sharedCount < 1)
        {
            delete _p;
            _p = nullptr;

            delete _refCnt; // 强引用==0直接delete
            _refCnt = nullptr;
        }
    }

private:
    T *_p;
    Counter *_refCnt;
};

template <class T>
class MyWeakPtr
{
public:
    MyWeakPtr()
        : _p(0), _refCnt(new Counter())
    {
        cout << "MyWeakPtr()" << endl;
    }
    MyWeakPtr(MySharedPtr<T> &msp)
        : _p(msp._p), _refCnt(msp._refCnt)
    {
        ++_refCnt->_weakCount;
        cout << "MyWeakPtr(MySharedPtr())" << endl;
    }
    MyWeakPtr(MyWeakPtr<T> &mwp)
        : _p(mwp._p), _refCnt(mwp._refCnt)
    {
        ++_refCnt->_weakCount;
        cout << "MyWeakPtr(MyWeakPtr())" << endl;
    }
    ~MyWeakPtr()
    {
        release();
        cout << "~MyWeakPtr()" << endl;
    }

    MyWeakPtr<T> &operator=(const MySharedPtr<T> &msp)
    {
        release();
        _p = msp._p;
        _refCnt = msp._refCnt;
        ++_refCnt->_weakCount;
        return *this;
    }
    MyWeakPtr<T> &operator=(const MyWeakPtr<T> &mwp)
    {
        if (this != &mwp)
        {
            release();
            _p = mwp._p;
            _refCnt = mwp._refCnt;
            ++_refCnt->_weakCount;
        }
        return *this;
    }
    MySharedPtr<T> lock()
    {
        if (!expired())
            return MySharedPtr<T>(*this);
        return MyWeakPtr<T>();
    }
    // 对象生命周期是否结束
    bool expired()
    {
        if (_refCnt)
        {
            if (_refCnt->_sharedCount > 0)
                return false;
        }
        return true;
    }
    int use_count() const { return _refCnt->_sharedCount; }

private:
    T &operator*() = delete;
    T *operator->() = delete;
    void release()
    {
        if (_refCnt) // 判断Counter对象是否还在
        {
            --_refCnt->_weakCount;
            if (_refCnt->_weakCount < 1 && _refCnt->_sharedCount < 1)
            {
                delete _refCnt;
                _refCnt = nullptr;
            }
        }
    }

private:
    T *_p;
    Counter *_refCnt;
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

void test0() // std::shared_ptr
{
    shared_ptr<Point> sp(new Point(1, 2));
    (*sp).print();
    sp->print();

    shared_ptr<Point> sp2(sp);
    sp2->print();
    sp->print();
    cout << sp.use_count() << endl; //引用计数
}

void test1() // MySharedPtr
{
    MySharedPtr<Point> sp(new Point(1, 2));
    (*sp).print();
    sp->print();

    MySharedPtr<Point> sp2(sp);
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

    shared_ptr<Child> _pChild_std;
    MySharedPtr<Child> _pChild_my;
};

class Child
{
public:
    Child() { cout << "Child()" << endl; }
    ~Child() { cout << "~Child()" << endl; }

    // shared_ptr<Parent> _pParent_std;
    weak_ptr<Parent> _pParent_std;

    // MySharedPtr<Parent> _pParent_my;
    MyWeakPtr<Parent> _pParent_my;
};

void test2() // shared_ptr 循环引用 // weak_ptr 解决循环引用
{
    shared_ptr<Child> pChild(new Child());
    shared_ptr<Parent> pParent(new Parent());
    cout << pChild.use_count() << endl;  // 1
    cout << pParent.use_count() << endl; // 1

    pParent->_pChild_std = pChild;       // 引用计数+1
    pChild->_pParent_std = pParent;      // 引用计数+1 --> weak_ptr 引用计数不变
    cout << pChild.use_count() << endl;  // 2
    cout << pParent.use_count() << endl; // 2 --> 1
}

void test3() // MySharedPtr MyWeakPtr
{
    MySharedPtr<Child> pChild(new Child());
    MySharedPtr<Parent> pParent(new Parent());
    cout << pChild.use_count() << endl;  // 1
    cout << pParent.use_count() << endl; // 1

    pParent->_pChild_my = pChild;        // 引用计数+1
    pChild->_pParent_my = pParent;       // 引用计数+1 --> weak_ptr 引用计数不变
    cout << pChild.use_count() << endl;  // 2
    cout << pParent.use_count() << endl; // 2 --> 1
}

int main()
{
    cout << "----------std::shared_ptr/weak_ptr----------" << endl;
    test0();
    test2();
    cout << "------------Myshared_ptr/weak_ptr-----------" << endl;
    test1();
    test3();
    return 0;
}