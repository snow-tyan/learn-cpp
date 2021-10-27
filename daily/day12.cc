/**
 *  MVC model-view-controler
 *  model      数据
 *  controller 业务逻辑
 *  view       界面
 *
 * **/

#include <iostream>
#include <memory>
#include <string>
#include <map>

using namespace std;

// 学生数据
class Student
{
public:
    Student(int id, string name) : _id(id), _name(name) {}
    int getId() const { return _id; }
    string getName() const { return _name; }

private:
    int _id;
    string _name;
};

// 数据展示
class View
{
public:
    View(Student *pstu = nullptr) : _pstu(pstu) {}
    void reset(Student *pstu)
    {
        _pstu = pstu;
    }

    virtual void show()
    {
        cout << "id: " << _pstu->getId() << "\tname: " << _pstu->getName() << endl;
    }

protected:
    Student *_pstu;
};

// 业务处理逻辑 控制器
class Controller
{
public:
    void attach(const Student &stu)
    {
        auto it = _students.find(stu.getId());
        if (it == _students.end())
        {
            _students.insert(std::make_pair(stu.getId(), stu));
        }
    }

    void detach(int id)
    {
        auto it = _students.find(id);
        if (it != _students.end())
        {
            _students.erase(id);
        }
    }

    void show(View *view)
    {
        for (auto &elem : _students)
        {
            view->reset(&elem.second);
            view->show();
        }
    }

private:
    map<int, Student> _students;
};

void test0()
{
    Controller ctrl;
    ctrl.attach(Student(100, "xiaoming"));
    ctrl.attach(Student(101, "daxiong"));
    ctrl.attach(Student(102, "jingxiang"));

    unique_ptr<View> myview(new View());
    ctrl.show(myview.get());
}

class SubView : public View
{
public:
    void show() override
    {
        cout << "[id: " << _pstu->getId() << "]"
             << " [name: " << _pstu->getName() << "]" << endl;
    }
};

void test1()
{
    Controller ctrl;
    ctrl.attach(Student(100, "xiaoming"));
    ctrl.attach(Student(101, "daxiong"));
    ctrl.attach(Student(102, "jingxiang"));

    unique_ptr<View> myview(new SubView());
    ctrl.detach(102);
    ctrl.show(myview.get());
}

int main()
{
    test0();
    test1();
    return 0;
}