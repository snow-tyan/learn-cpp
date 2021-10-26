// Stack
#include <iostream>
using namespace std;

class Stack
{
public:
    Stack(int capacity)
        : _capacity(capacity), _data(new int[capacity]()), _top(-1)
    {
        cout << "Stack(int)" << endl;
    }
    ~Stack()
    {
        cout << "~Stack()" << endl;
        if (_data)
            delete _data;
    }
    void push(int);
    void pop();
    int top();
    bool empty() const;
    bool full() const;

private:
    int _capacity; //构造传值
    int *_data;
    int _top; //栈顶指针
};

void Stack::push(int val)
{
    if (!full())
        _data[++_top] = val;
    else
        cout << "Stack is full" << endl;
}
int Stack::top()
{
    if (!empty())
        return _data[_top];
    else
        cout << "Stack is empty" << endl;
}
void Stack::pop()
{
    if (!empty())
        --_top;
    else
        cout << "Stack is empty" << endl;
}
bool Stack::empty() const
{
    if (_top == -1)
        return true;
    return false;
}
bool Stack::full() const
{
    if (_top + 1 == _capacity)
        return true;
    return false;
}

//循环队列
class Queue
{
public:
    Queue(int capacity)
        : _capacity(capacity + 1), _data(new int[capacity + 1]()), _front(0), _rear(0)
    {
        cout << "Queue(int)" << endl;
    }

    ~Queue()
    {
        cout << "~Queue()" << endl;
        if (_data)
            delete _data;
    }
    void push_back(int);
    void pop_front();
    int front() const;
    int back() const;
    bool empty() const;
    bool full() const;

private:
    int _front; //队头指针
    int _rear;  //尾后指针，希望进队、出队都是O(1)复杂度
    int _capacity;
    int *_data;
};
bool Queue::empty() const
{
    return _front == _rear;
}
bool Queue::full() const
{
    return (_rear + 1) % _capacity == _front;
}
void Queue::push_back(int val)
{
    if (!full())
    {
        _data[_rear++] = val;
        _rear %= _capacity;
    }
    else
        cout << "Queue is full" << endl;
}
void Queue::pop_front()
{
    if (!empty())
    {
        ++_front;
        _front %= _capacity;
    }
}
int Queue::front() const
{
    if (!empty())
        return _data[_front];
    else
        cout << "Queue is empty" << endl;
}
int Queue::back() const
{
    if (!empty())
        return _data[(_rear - 1 + _capacity) % _capacity];
    else
        cout << "Queue is empty" << endl;
}

int main()
{
    Stack stk(3);
    stk.push(1);
    stk.push(2);
    cout << stk.top() << endl; // 2
    stk.push(3);
    stk.push(4); // full
    stk.pop();
    stk.push(4);
    cout << stk.top() << endl;

    Queue que(3);
    que.push_back(1);
    que.push_back(2);
    que.push_back(3);
    cout << que.front() << "  " << que.back() << endl;
    que.pop_front();
    cout << que.front() << "  " << que.back() << endl;
    que.push_back(4);
    cout << que.front() << "  " << que.back() << endl;
    que.push_back(5); // full

    return 0;
}