#pragma once

#include "TextQuery.hh"

using namespace std;

class Query_base
{
    friend class Query;

protected:
    virtual ~Query_base() {}

private:
    virtual QueryResult eval(const TextQuery &) const = 0;
    virtual string rep() const = 0;
};

class Query
{
    friend Query operator~(const Query &);
    friend Query operator&(const Query &, const Query &);
    friend Query operator|(const Query &, const Query &);

public:
    Query(string &);
    QueryResult eval(const TextQuery &tq) const { return _q->eval(tq); }
    string rep() const { return _q->rep(); }

private:
    Query(shared_ptr<Query_base> query) : _q(query) {}

private:
    shared_ptr<Query_base> _q;
};

class WordQuery
    : public Query_base
{
private:
    friend class Query;
    WordQuery(string word) : _word(word) {}
    QueryResult eval(const TextQuery &tq) const { return tq.query(_word); }
    string rep() const { return _word; }

private:
    string _word;
};

// Query 构造函数
inline Query::Query(string &s) : _q(new WordQuery(s)) {}

class NotQuery
    : public Query_base
{
private:
    friend Query operator~(const Query &);
    NotQuery(const Query q) : _query(q) {}
    QueryResult eval(const TextQuery &) const;
    string rep() const { return "~(" + _query.rep() + ")"; }

private:
    Query _query;
};

class BinaryQuery
    : public Query_base
{
protected:
    BinaryQuery(const Query &lhs, const Query &rhs, string opName)
        : _lhs(lhs), _rhs(rhs), _opName(opName) {}
    string rep() const { return "(" + _lhs.rep() + " " + _opName + " " + _rhs.rep() + ")"; }

protected:
    Query _lhs;
    Query _rhs;
    string _opName; // 运算符名称 '&' / '|'
};

class AndQuery
    : public BinaryQuery
{
private:
    friend Query operator&(const Query &, const Query &);
    AndQuery(const Query &lhs, const Query &rhs)
        : BinaryQuery(lhs, rhs, "&") {}
    QueryResult eval(const TextQuery &) const;
};

class OrQuery
    : public BinaryQuery
{
private:
    friend Query operator|(const Query &, const Query &);
    OrQuery(const Query &lhs, const Query &rhs)
        : BinaryQuery(lhs, rhs, "|") {}
    QueryResult eval(const TextQuery &) const;
};

inline Query operator~(const Query &operand)
{
    return shared_ptr<Query_base>(new NotQuery(operand));
}
inline Query operator&(const Query &lhs, const Query &rhs)
{
    return shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}
inline Query operator|(const Query &lhs, const Query &rhs)
{
    return shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}