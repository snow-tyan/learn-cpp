#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
using namespace std;

void printVec(vector<int> &nums)
{
    for (int &num : nums)
        cout << num << " ";
    cout << endl;
}
class Solution
{
public:
    /**
     * lru design
     * @param operators int整型vector<vector<>> the ops
     * @param k int整型 the k
     * @return int整型vector
     **/
    using PAIR = pair<int, int>;
    vector<int> LRU(vector<vector<int>> &operators, int k)
    {
        // write code here
        LRUCache cache(k);
        vector<int> res;
        for (auto &opt : operators)
        {
            if (opt[0] == 1)
            {
                cache.put(opt[1], opt[2]);
            }
            else
            {
                res.push_back(cache.get(opt[1]));
            }
        }
        return res;
    }

    class LRUCache
    {
    public:
        LRUCache(int capacity) : _capacity(capacity) {}
        int get(int key)
        {
            auto it = _cache.find(key);
            if (it != _cache.end())
            {
                int ret = it->second->second;
                _list.splice(_list.begin(), _list, it->second);
                _cache[key] = _list.begin();
                return ret;
            }
            return -1;
        }

        void put(int key, int value)
        {
            auto it = _cache.find(key);
            if (it != _cache.end())
            {
                it->second->second = value;
                _list.splice(_list.begin(), _list, it->second);
                _cache[key] = _list.begin();
            }
            else
            {
                if (_list.size() == _capacity)
                {
                    int k = _list.back().first;
                    _list.pop_back();
                    _cache.erase(k);
                }
                _list.push_front(PAIR(key, value));
                _cache[key] = _list.begin();
            }
        }

    private:
        int _capacity;
        list<PAIR> _list;
        unordered_map<int, list<PAIR>::iterator> _cache;
    };
};

int main()
{
    Solution solve;
    vector<vector<int>> operators = {{1, 1, 1}, {1, 2, 2}, {1, 3, 2}, {2, 1}, {1, 4, 4}, {2, 2}};
    vector<int> res = solve.LRU(operators, 3);
    printVec(res);
    return 0;
}