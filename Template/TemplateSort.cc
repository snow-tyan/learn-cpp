// Using Template to rewrite sort.cc

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

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

template <class T, class Compare = std::less<T>>
class HeapSort
{
public:
    void sort(vector<T> &);

private:
    void adjustHeap(vector<T> &, int, int);
};

template <class T, class Compare>
void HeapSort<T, Compare>::adjustHeap(vector<T> &nums, int parent, int length)
{
    int son = 2 * parent + 1;
    while (son < length)
    {
        if (son + 1 < length && Compare()(nums[son], nums[son + 1]))
            ++son;
        if (Compare()(nums[son], nums[parent]))
            break;
        swap(nums[son], nums[parent]);
        parent = son;
        son = 2 * parent + 1;
    }
}

template <class T, class Compare>
void HeapSort<T, Compare>::sort(vector<T> &nums)
{
    // build heap O(N)
    for (int i = nums.size() / 2 - 1; i >= 0; --i)
        adjustHeap(nums, i, nums.size());
    swap(nums[0], nums[nums.size() - 1]);
    // break and adjust heap
    for (int i = 1; i < nums.size(); ++i)
    {
        adjustHeap(nums, 0, nums.size() - i);
        swap(nums[0], nums[nums.size() - i - 1]);
    }
}

int main()
{
    vector<int> nums = {3, 5, 1, 2, 8, 6, 7, 9, 4};
    print(nums);
    HeapSort<int> hs;
    // HeapSort<int, std::greater<int>> hs;
    hs.sort(nums);
    print(nums);

    vector<string> vstr = {"bbb", "ddd", "aaa", "ccc"};
    print(vstr);
    // HeapSort<string> shs;
    HeapSort<string, std::greater<string>> shs;
    shs.sort(vstr);
    print(vstr);

    return 0;
}