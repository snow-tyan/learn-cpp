// 观察者模式

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <list>
#include <algorithm>
#include <string>
#include <memory>
using namespace std;

class Observer
{
public:
    // void update(Subject *);  拉模式 观察者主动推送信息
    // void update(Message);    推模式 观察者被动接受信息
    virtual void update() = 0;
    virtual ~Observer() { cout << "~Observer()" << endl; }
};

class Baby
    : public Observer
{
public:
    Baby(const string &name) : _name(name) {}
    void update();

private:
    string _name;
};

class Nurse
    : public Observer
{
public:
    Nurse(const string &name) : _name(name) {}
    void update();

private:
    string _name;
};

class Subject
{
public:
    virtual void attach(Observer *) = 0;
    virtual void detach(Observer *) = 0;
    virtual void notify() = 0;
    virtual ~Subject() { cout << "~Subject()" << endl; }
};
// 门铃
class Ring
    : public Subject
{
public:
    Ring() : _isAlarming(false) {}
    void attach(Observer *);
    void detach(Observer *);
    void notify();

    // 按下门铃
    void isPressed();

private:
    // 可以对观察者进行分类，如黄金用户、白金用户
    list<Observer *> _obList;
    bool _isAlarming;
};

class Guest
{
public:
    Guest(const string &name) : _name(name) {}
    void knock(Ring &);

private:
    string _name;
};

void Guest::knock(Ring &ring)
{
    cout << ">> Guest " << _name << " is knocking at the door!" << endl;
    ring.isPressed();
}

void Ring::isPressed()
{
    if (!_isAlarming)
    {
        _isAlarming = true;
        notify(); // 通知其他观察者
    }
    _isAlarming = false;
}

void Ring::attach(Observer *obj)
{
    auto it = std::find(_obList.begin(), _obList.end(), obj);
    if (it == _obList.end())
    {
        // 没找到，添加到list中
        _obList.push_back(obj);
    }
}

void Ring::detach(Observer *obj)
{
    auto it = std::find(_obList.begin(), _obList.end(), obj);
    if (it != _obList.end())
    {                      // 找到了
        _obList.erase(it); // 参数是迭代器
    }
}

void Ring::notify()
{
    for (auto &obj : _obList)
    {
        obj->update();
    }
}

void Baby::update()
{
    ::srand(::clock());
    int number = ::rand() % 100;
    if (number < 70)
    {
        cout << "Baby " << _name << " is sleeping~" << endl;
    }
    else
    {
        cout << "Baby " << _name << " is starting crying!" << endl;
    }
}

void Nurse::update()
{
    ::srand(::clock());
    int number = ::rand() % 100;
    if (number < 30)
    {
        cout << "Nurse " << _name << " is sleeping~" << endl;
    }
    else
    {
        cout << "Nurse " << _name << " is awake, and is ready to open the door!" << endl;
    }
}

void test()
{
    unique_ptr<Observer> baby1(new Baby("A"));
    unique_ptr<Observer> baby2(new Baby("B"));
    unique_ptr<Observer> nurse1(new Nurse("a"));
    unique_ptr<Observer> nurse2(new Nurse("b"));

    Ring ring;
    ring.attach(baby1.get());
    ring.attach(baby2.get());
    ring.attach(nurse1.get());
    ring.attach(nurse2.get());

    Guest guest("William");
    guest.knock(ring);
    cout << "----------------------------------------------------" << endl;
    ring.detach(nurse1.get());
    guest.knock(ring);
}

int main()
{
    test();
    return 0;
}