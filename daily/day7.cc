// 流迭代器
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

template <class Container>
void print(Container &c)
{
    copy(c.begin(), c.end(), ostream_iterator<typename Container::value_type>(cout, " "));
    cout << endl;
}

void test0()
{
    vector<int> nums = {1, 2, 3, 4, 5};
    ostream_iterator<int> osi(cout, " "); //流对象，分隔符
    copy(nums.begin(), nums.end(), osi);  // std::copy(Input first, Input last, Output first);
}

void test1()
{
    vector<int> nums;
    istream_iterator<int> isi(cin);
    // copy(isi, istream_iterator<int>(), nums.begin());//error begin(),end()是用来遍历的，不能用来插入
    copy(isi, istream_iterator<int>(), back_inserter(nums)); // ctrl+d才会结束 内部调用nums.push_back()
    copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " "));
}

void test2()
{
    vector<int> nums = {1, 2, 3, 4, 5};
    // reverse_iterator<vector<int>::iterator> rit(nums.rbegin());
    vector<int>::reverse_iterator rit = nums.rbegin();
    for (; rit != nums.rend(); ++rit)
    {
        cout << *rit << " ";
    }
    cout << endl;
}

int main()
{
    // test0();
    // test1();
    // test2();

    return 0;
}