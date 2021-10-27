#pragma once

#include "QueryResult.hh"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>

using namespace std;

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
    static string cleanup_str(const string&); // 忽略标点大小写

private:
    shared_ptr<vector<string>> _file;
    map<string, shared_ptr<set<int>>> _map;
};
