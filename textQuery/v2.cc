// 简单的文本查询
/*  查询某个单词在文件中出现次数及其所在行号，
    如果单词在一行出现多次，只列出一次，行号按
    升序列出  */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>

using namespace std;

// 将查询结果封成一个类
// 查询结果有：该单词出现了多少次、行号、内容
// 类之间共享数据，返回指向TextQuery对象内部的迭代器或指针
// 但是若TextQuery对象在QueryResult之前销毁会导致错误
// --> 交给智能指针shared_ptr托管
class QueryResult;

class TextQuery
{
public:
    TextQuery(ifstream &);
    QueryResult query(const string &) const;
    void checkFile() const
    {
        for (const string &line : *_file)
            cout << line << endl;
    }

private:
    shared_ptr<vector<string>> _file;
    map<string, shared_ptr<set<int>>> _map;
};

class QueryResult
{
public:
    QueryResult(string word, shared_ptr<set<int>> lines,
                shared_ptr<vector<string>> file)
        : _word(word), _lines(lines), _file(file) {}
    friend ostream &print(ostream &, const QueryResult &);

private:
    string _word;                     // 查询的单词
    shared_ptr<set<int>> _lines;      // 行号集合，即map查询结果
    shared_ptr<vector<string>> _file; // 文本
};

TextQuery::TextQuery(ifstream &ifs)
    : _file(new vector<string>)
{
    string line;
    int lineNum = 0;
    while (getline(ifs, line))
    {
        _file->push_back(line);
        ++lineNum;
        istringstream iss(line);
        string word;
        while (iss >> word)
        {
            auto &lineSet = _map[word]; // lineSet是个shared_ptr
            if (!lineSet)               // 指针为空，申请一个set
                lineSet.reset(new set<int>);
            lineSet->insert(lineNum);
        }
    }
    cout << "读取文件成功，共" << lineNum << "行" << endl;
}

QueryResult TextQuery::query(const string &word) const
{
    static shared_ptr<set<int>> nodata(new set<int>); //指向空set的shared_ptr
    auto it = _map.find(word);
    if (it != _map.end())
        return QueryResult(word, it->second, _file);
    return QueryResult(word, nodata, _file);
}

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

void runQueries(ifstream &infile)
{
    TextQuery tq(infile);
    // tq.checkFile();
    while (true)
    {
        cout << "请输入要查询的单词，输入q退出" << endl;
        string s;
        if (!(cin >> s) || s == "q")
            break;
        print(cout, tq.query(s)) << endl;
    }
}

int main()
{
    ifstream ifs("china_daily.txt");
    runQueries(ifs);
    ifs.close();
    return 0;
}