#include <math.h>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <unordered_set>
#include <vector>
#include <queue>
using namespace std;

template <class T>
void printMp(T &t)
{
    typename T::const_iterator cit = t.begin();
    for (; cit != t.end(); ++cit)
    {
        cout << cit->first << "-->" << cit->second << endl;
    }
}

template <class T>
void print(T &t)
{
    typename T::const_iterator cit = t.begin();
    for (; cit != t.end(); ++cit)
    {
        cout << *cit << " ";
    }
    cout << endl;
}

void test0()
{
    set<int> st = {1, 1, 1, 2, 3, 4, 5};
    multiset<int> mst = {1, 1, 1, 2, 3, 4, 5};
    auto ret = st.insert(6);
    if (ret.second)
        cout << "success" << endl;
    else
        cout << "failed" << endl;

    cout << st.count(1) << endl;
    cout << *st.find(1) << endl;
    cout << mst.count(1) << endl;
    auto it = mst.find(1);
    cout << *it << endl;
    ++it;
    cout << *it << endl;
    ++it;
    cout << *it << endl;
    ++it;
    cout << *it << endl;
}

void test1()
{
    map<int, string> mp = {
        make_pair(1, "Beijing"),
        make_pair(2, "Shanghai"),
        make_pair(3, "Guangzhou"),
        make_pair(4, "Shenzhen"),
    };

    mp.insert(make_pair(4, "Hangzhou")); //添加失败
    printMp(mp);

    mp[4] = "Hangzhou"; //修改
    printMp(mp);
    mp[5] = "Nanjing"; // 添加
    printMp(mp);
}
void test2()
{
    multimap<int, string> mp = {
        make_pair(1, "Beijing"),
        make_pair(2, "Shanghai"),
        make_pair(3, "Guangzhou"),
        make_pair(4, "Shenzhen"),
    };

    mp.insert(make_pair(4, "Hangzhou"));
    printMp(mp);
}

void test3()
{
    vector<int> nums = {5, 7, 3, 4, 1, 9, 8, 6, 2};
    unordered_set<int> unst(nums.begin(), nums.end());
    print(unst);
}

class Point
{
public:
    Point() = default;
    Point(int ix, int iy) : _ix(ix), _iy(iy) {}
    double getDistance() const { return sqrt(_ix * _ix + _iy * _iy); }
    int getX() const { return _ix; }
    int getY() const { return _iy; }
    friend ostream &operator<<(ostream &os, const Point &pt);

private:
    int _ix = 0;
    int _iy = 0;
};
ostream &operator<<(ostream &os, const Point &pt)
{
    return os << "(" << pt._ix << ","
              << pt._iy << ")";
}
bool operator==(const Point &pt1, const Point &pt2)
{
    return pt1.getX() == pt2.getX() && pt1.getY() == pt2.getY();
}
namespace std
{
    template <>
    struct hash<Point> //实现hash函数
    {
        size_t operator()(const Point &pt) const
        {
            return (pt.getX() * pt.getX() - 1) ^ (pt.getY() * pt.getY() - 1);
        }
    };
} // namespace std

//使用自己的Hash
struct PointHasher
{
    size_t operator()(const Point &pt) const
    {
        return (pt.getX() * pt.getX() - 1) ^ (pt.getY() * pt.getY() - 1);
    }
};

//使用自己的比较规则
struct PointEqual
{
    bool operator()(const Point &pt1, const Point &pt2) const
    {
        return pt1.getX() == pt2.getX() && pt1.getY() == pt2.getY();
    }
};

void test4()
{
    unordered_set<Point, PointHasher, PointEqual> unst{
        Point(1, 1),
        Point(2, 2),
        Point(3, 3),
        Point(),
    };
    print(unst);
}

void test5()
{
    vector<int> nums = {5, 7, 3, 4, 1, 9, 8, 6, 2};
    priority_queue<int> pque(nums.begin(), nums.end()); //默认小于符号比较，大顶堆
    while (!pque.empty())
    {
        cout << pque.top() << " ";
        pque.pop();
    }
    cout << endl;
}

void test6()
{
    vector<int> nums = {5, 7, 3, 4, 1, 9, 8, 6, 2};
    priority_queue<int> pque;
    for (auto it = nums.begin(); it != nums.end(); ++it)
    {
        pque.push(*it);
    }
    while (!pque.empty())
    {
        cout << pque.top() << " ";
        pque.pop();
    }
    cout << endl;
}
struct PointCompare
{
    bool operator()(const Point &pt1, const Point &pt2) const
    {
        return pt1.getDistance() < pt2.getDistance(); //小于符号 大顶堆；大于符号，小顶堆
    }
};
void test7() //自定义类型
{
    vector<Point> vpt = {Point(1, 1), Point(1, 2), Point(2, 1), Point(2, 2), Point(3, 3), Point()};
    priority_queue<Point, vector<Point>, PointCompare> pqpt(vpt.begin(), vpt.end());
    while (!pqpt.empty())
    {
        cout << pqpt.top() << " ";
        pqpt.pop();
    }
    cout << endl;
}

void test8()
{
    unordered_set<int> unset(100); // 人为指定桶数量
    for (int i = 0; i < 20; ++i)
    {
        unset.insert(i*i);
        cout << unset.size() << " " << unset.bucket_count() << endl;
    }
}
int main()
{
    // test0();
    // test1();
    // test2();
    // test3();
    // test4();
    // test5();
    // test6();
    // test7();
    test8();

    return 0;
}