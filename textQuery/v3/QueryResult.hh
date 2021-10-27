#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <string>

using namespace std;

class QueryResult
{
public:
    QueryResult(string word, shared_ptr<set<int>> lines, shared_ptr<vector<string>> file)
        : _word(word), _lines(lines), _file(file) {}
    set<int>::size_type size() const { return _lines->size(); }
    set<int>::const_iterator begin() const { return _lines->begin(); }
    set<int>::const_iterator end() const { return _lines->end(); }
    shared_ptr<vector<string>> getFile() { return _file; }

    friend ostream &print(ostream &, const QueryResult &);

private:
    string _word;                     // 单词
    shared_ptr<set<int>> _lines;      // 行号
    shared_ptr<vector<string>> _file; // 整个文本
};

ostream &print(ostream &os, const QueryResult &qr)
{
    os << qr._word << "共出现" << qr._lines->size() << "次" << endl;
    for (auto &num : *qr._lines)
    {
        os << "\t(第" << num << "行) "
           << *(qr._file->begin() + num - 1) << endl;
    }
    return os;
}
