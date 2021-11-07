/**
 * 简易版本vector，学习allocator基本用法
 *
 * @file MyVector.cc
 * @author snow-tyan (zziywang@163.com)
 * @brief {Life is too short to learn cpp.}
 * @version 0.1
 * @date 2021-11-07
 * 
 * @copyright Copyright (c) 2021
 *
 */

#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

template <typename T>
void print(const T &t)
{
    typename T::const_iterator it = t.begin();
    for (; it != t.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

template <class T>
class MyVector
{
public:
    using iterator = T *;
    using const_iterator = const T *;

    MyVector();
    ~MyVector();
    void push_back(const T &);
    void pop_back();
    size_t size() const;
    size_t capacity() const;
    // 迭代器
    const_iterator cbegin() const;
    const_iterator cend() const;
    const_iterator begin() const;
    const_iterator end() const;
    iterator begin();
    iterator end();
    // 下标访问
    T &operator[](size_t);

private:
    void reallocate();

private:
    static std::allocator<T> _alloc;
    iterator _start;
    iterator _finish;
    iterator _endOfStorge;
};
template <class T>
std::allocator<T> MyVector<T>::_alloc;

template <class T>
MyVector<T>::MyVector()
    : _start(nullptr),
      _finish(nullptr),
      _endOfStorge(nullptr) {}

template <class T>
MyVector<T>::~MyVector()
{
    if (_start) {
        // 删除对象并回收已分配的空间
        while (_finish != _start) {
            _alloc.destroy(--_finish);
        }
        _alloc.deallocate(_start, capacity());
    }
}

template <class T>
void MyVector<T>::push_back(const T &val)
{
    if (size() == capacity()) {
        reallocate();
    }
    _alloc.construct(_finish++, val);
}

template <class T>
void MyVector<T>::pop_back()
{
    if (size() != 0) {
        _alloc.destroy(--_finish);
    }
}

template <class T>
size_t MyVector<T>::size() const
{
    return _finish - _start;
}

template <class T>
size_t MyVector<T>::capacity() const
{
    return _endOfStorge - _start;
}

template <class T>
typename MyVector<T>::const_iterator MyVector<T>::cbegin() const { return _start; }
template <class T>
typename MyVector<T>::const_iterator MyVector<T>::cend() const { return _finish; }
template <class T>
typename MyVector<T>::const_iterator MyVector<T>::begin() const { return _start; }
template <class T>
typename MyVector<T>::const_iterator MyVector<T>::end() const { return _finish; }
template <class T>
typename MyVector<T>::iterator MyVector<T>::begin() { return _start; }
template <class T>
typename MyVector<T>::iterator MyVector<T>::end() { return _finish; }

template <class T>
T &MyVector<T>::operator[](size_t idx)
{
    // 如果下标不合法
    if (idx < 0 || idx >= size()) {
        __throw_out_of_range("下标越界哦");
    }
    return _start[idx];
}

template <typename T>
void MyVector<T>::reallocate()
{
    size_t oldCapacity = capacity();
    size_t newCapacity = oldCapacity > 0 ? 2 * oldCapacity : 1;

    // 空间分配
    T *ptemp = _alloc.allocate(newCapacity);
    if (_start) {
        // std::copy(_start, _finish, ptemp);
        // std::copy内部调用对象的赋值运算符，然而对象还不存在
        std::uninitialized_copy(_start, _finish, ptemp);

        // 删除原空间对象并回收空间
        while (_finish != _start) {
            _alloc.destroy(--_finish);
        }
        _alloc.deallocate(_start, oldCapacity);
    }
    // 底层三指针
    _start = ptemp;
    _finish = ptemp + oldCapacity;
    _endOfStorge = ptemp + newCapacity;
}

void test0()
{
    vector<int> nums;
    cout << "vector's capacity=" << nums.capacity() << endl;
    for (int i = 0; i < 10; ++i) {
        nums.push_back(i);
        cout << "vector's size=" << nums.size() << endl;
        cout << "vector's capacity=" << nums.capacity() << endl;
    }
    print(nums);
    nums.pop_back();
    print(nums);
    cout << "vector's size=" << nums.size() << endl;
    cout << "vector's capacity=" << nums.capacity() << endl;
    // cout << nums[-1] << endl;
    cout << nums.at(9) << endl;
}

void test1()
{
    MyVector<int> nums;
    cout << "vector's capacity=" << nums.capacity() << endl;
    for (int i = 0; i < 10; ++i) {
        nums.push_back(i);
        cout << "vector's size=" << nums.size() << endl;
        cout << "vector's capacity=" << nums.capacity() << endl;
    }
    print(nums);
    nums.pop_back();
    print(nums);
    cout << "vector's size=" << nums.size() << endl;
    cout << "vector's capacity=" << nums.capacity() << endl;
    cout << nums[9] << endl; // 越界
}

int main()
{
    cout << "std::vector" << endl;
    // test0();
    cout << "MyVector" << endl;
    test1();

    return 0;
}