#include "TextQuery.hh"
#include <ctype.h>
#include <sstream>

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
            word = cleanup_str(word);
            auto &lineSet = _map[word]; // lineSet是个shared_ptr
            if (!lineSet)               // 指针为空，申请一个set
                lineSet.reset(new set<int>);
            lineSet->insert(lineNum);
        }
    }
    cout << "读取文件成功，共" << lineNum << "行" << endl;
}

string TextQuery::cleanup_str(const string &word)
{
    string cleanWord;
    for (auto it = word.begin(); it != word.end(); ++it)
    {
        if (!ispunct(*it)) // 非标点
            cleanWord += tolower(*it); // 转小写
    }
    return cleanWord;
}

QueryResult TextQuery::query(const string &word) const
{
    static shared_ptr<set<int>> nodata(new set<int>); //指向空set的shared_ptr
    auto it = _map.find(word);
    if (it != _map.end())
        return QueryResult(word, it->second, _file);
    return QueryResult(word, nodata, _file);
}


