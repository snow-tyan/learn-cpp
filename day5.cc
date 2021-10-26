#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
using namespace std;

template <typename T>
void print(const T &t)
{
    typename T::const_iterator cit = t.begin();
    for (; cit != t.end(); ++cit)
    {
        cout << *cit << " ";
    }
    cout << endl;
}

template <class T>
class Greater
{
public:
    bool operator()(const T &lhs, const T &rhs)
    {
        return lhs > rhs;
    }
};

int main()
{
    vector<int> nums = {1, 2, 3, 4, 5};
    list<int> dblist = {2, 3, 4, 5, 6};
    deque<int> deq = {3, 4, 5, 6, 7, 8, 9};
    print(nums);
    print(dblist);
    print(deq);

    std::greater<int> gt;
    // Greater<int> gt;
    dblist.sort(gt);
    print(dblist);

    set<int, Greater<int>> numSet(deq.begin(), deq.end()); //默认升序 无重复
    numSet.insert(1);
    numSet.insert(2);
    numSet.insert(3);
    numSet.insert(4);
    print(numSet);

    return 0;
}