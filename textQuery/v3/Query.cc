#include "Query.hh"

QueryResult NotQuery::eval(const TextQuery &tq) const
{
    QueryResult result = _query.eval(tq);
    shared_ptr<set<int>> retLines(new set<int>);
    auto begin = result.begin(), end = result.end();
    auto fileSize = result.getFile()->size();
    for (int i = 0; i != fileSize; ++i)
    {
        if (begin == end || *begin != i)
        {
            retLines->insert(i);
        }
        else if (begin != end)
        {
            ++begin;
        }
    }
    return QueryResult(rep(), retLines, result.getFile());
}

QueryResult AndQuery::eval(const TextQuery &tq) const
{
}

QueryResult OrQuery::eval(const TextQuery &tq) const
{
}