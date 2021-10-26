// 简单的文本查询
/*  查询某个单词在文件中出现次数及其所在行号，
    如果单词在一行出现多次，只列出一次，行号按
    升序列出  */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class TextQuery
{
public:
    void readFile(ifstream &);
    void query(const string &) const;
    void print() const
    {
        for (const string &line : _file)
            cout << line << endl;
    }

private:
    vector<string> _file;       // 整个文件，以行分割，下标即行号
    map<string, set<int>> _map; // key是单词，val是行号集合，要求不重复故用set
};

void TextQuery::readFile(ifstream &ifs)
{
    string line;
    int lineNum = 0;
    while (getline(ifs, line))
    {
        _file.push_back(line);
        ++lineNum;
        istringstream iss(line); // 字符串输入流
        string word;
        while (iss >> word) // 按空格分割行
        {
            auto &lineSet = _map[word];
            lineSet.insert(lineNum);
        }
    }
    cout << "读取文件成功，共" << lineNum << "行" << endl;
}

void TextQuery::query(const string &word) const
{
    auto it = _map.find(word);
    if (it != _map.end())
    {
        cout << "查询完毕，单词";
        cout << it->first << "共出现"
             << it->second.size() << "次" << endl;
        for (auto &lineNum : it->second)
        {
            cout << "\t(第" << lineNum << "行) "
                 << _file[lineNum - 1] << endl;
        }
    }
    else
    {
        cout << "查无此词！" << endl;
    }
}

void runQueries(ifstream &infile)
{
    TextQuery tq;
    tq.readFile(infile);
    while (true)
    {
        cout << "请输入要查询的单词，输入q退出" << endl;
        string s;
        if (!(cin >> s) || s == "q")
            break;
        tq.query(s);
    }
}

void test0()
{
    ifstream ifs("china_daily.txt");
    runQueries(ifs);
    ifs.close();
}

void test1()
{
    TextQuery tq;
    ifstream ifs("china_daily.txt");
    tq.readFile(ifs);
    tq.print();
}

int main()
{
    test0();
    // test1();
    return 0;
}