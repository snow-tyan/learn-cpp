// Using Template Stack Queue
#include <iostream>
#include <string>
using namespace std;

template <typename T, int capacity = 10>
class Stack
{
public:
    Stack()
        : _data(new T[capacity]()), _top(-1)
    {
        cout << "Stack()" << endl;
    }
    ~Stack()
    {
        cout << "~Stack()" << endl;
        if (_data)
            delete[] _data;
    }
    void push(const T);
    void pop();
    T top();
    bool empty() const;
    bool full() const;

private:
    T *_data;
    int _top; //栈顶指针
};

template <typename T, int capacity>
void Stack<T, capacity>::push(const T val)
{
    if (!full())
        _data[++_top] = val;
    else
        cout << "Stack is full" << endl;
}

template <typename T, int capacity>
T Stack<T, capacity>::top()
{
    if (!empty())
        return _data[_top];
    else
        cout << "Stack is empty" << endl;
}
template <typename T, int capacity>
void Stack<T, capacity>::pop()
{
    if (!empty())
        --_top;
    else
        cout << "Stack is empty" << endl;
}
template <typename T, int capacity>
bool Stack<T, capacity>::empty() const
{
    if (_top == -1)
        return true;
    return false;
}
template <typename T, int capacity>
bool Stack<T, capacity>::full() const
{
    if (_top + 1 == capacity)
        return true;
    return false;
}

//循环队列
template <typename T, int capacity = 10>
class Queue
{
public:
    Queue()
        : _data(new T[capacity + 1]()), _front(0), _rear(0)
    {
        cout << "Queue()" << endl;
    }

    ~Queue()
    {
        cout << "~Queue()" << endl;
        if (_data)
            delete[] _data;
    }
    void push_back(const T);
    void pop_front();
    T front() const;
    T back() const;
    bool empty() const;
    bool full() const;

private:
    int _front; //队头指针
    int _rear;  //尾后指针，希望进队、出队都是O(1)复杂度
    T *_data;
};
template <typename T, int capacity>
bool Queue<T, capacity>::empty() const
{
    return _front == _rear;
}
template <typename T, int capacity>
bool Queue<T, capacity>::full() const
{
    return (_rear + 1) % capacity == _front;
}
template <typename T, int capacity>
void Queue<T, capacity>::push_back(const T val)
{
    if (!full())
    {
        _data[_rear++] = val;
        _rear %= capacity;
    }
    else
        cout << "Queue is full" << endl;
}
template <typename T, int capacity>
void Queue<T, capacity>::pop_front()
{
    if (!empty())
    {
        ++_front;
        _front %= capacity;
    }
}
template <typename T, int capacity>
T Queue<T, capacity>::front() const
{
    if (!empty())
        return _data[_front];
    else
        cout << "Queue is empty" << endl;
}
template <typename T, int capacity>
T Queue<T, capacity>::back() const
{
    if (!empty())
        return _data[(_rear - 1 + capacity) % capacity];
    else
        cout << "Queue is empty" << endl;
}

void testStackInt()
{
    Stack<int, 3> stk;
    stk.push(1);
    stk.push(2);
    cout << stk.top() << endl; // 2
    stk.push(3);
    stk.push(4); // full
    stk.pop();
    stk.push(4);
    cout << stk.top() << endl; // 4
}
void testStackString()
{
    Stack<string, 3> stk;
    stk.push("aaa");
    stk.push("bbb");
    cout << stk.top() << endl; // bbb
    stk.push("ccc");
    stk.push("ddd"); // full
    stk.pop();
    stk.push("ddd");
    cout << stk.top() << endl; // ddd
}

void testQueueInt()
{
    Queue<int, 4> que; //capacity+1
    que.push_back(1);
    que.push_back(2);
    que.push_back(3);
    cout << que.front() << "  " << que.back() << endl; // 1 3
    que.pop_front();
    cout << que.front() << "  " << que.back() << endl; // 2 3
    que.push_back(4);
    cout << que.front() << "  " << que.back() << endl; // 2 4
    que.push_back(5); // full
}
void testQueueString()
{
    Queue<string, 4> que;
    que.push_back("aaa");
    que.push_back("bbb");
    que.push_back("ccc");
    cout << que.front() << "  " << que.back() << endl; // aaa ccc
    que.pop_front();
    cout << que.front() << "  " << que.back() << endl; // bbb ccc
    que.push_back("ddd");
    cout << que.front() << "  " << que.back() << endl; // bbb ddd
    que.push_back("eee"); // full
}

int main()
{
    testStackInt();
    testStackString();
    testQueueInt();
    testQueueString();

    return 0;
}