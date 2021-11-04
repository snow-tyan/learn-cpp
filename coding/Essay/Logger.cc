//封装log4cpp

#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <iostream>
#include <string>

using namespace std;
using namespace log4cpp;

class MyLogger
{
public:
    static MyLogger &getInstance()
    {
        return _instance;
    }

    void error(const char *msg) const;
    void warn(const char *msg) const;
    void info(const char *msg) const;
    void debug(const char *msg) const;

private:
    MyLogger();
    ~MyLogger();

private:
    static MyLogger _instance;
    Category &_myCategory;
};
MyLogger MyLogger::_instance;

MyLogger::MyLogger()
    : _myCategory(Category::getRoot().getInstance("myCategory"))
{
    cout << "MyLogger()" << endl;
    PatternLayout *pLayout1 = new PatternLayout();
    pLayout1->setConversionPattern("%d [%p] %m%n");
    PatternLayout *pLayout2 = new PatternLayout();
    pLayout2->setConversionPattern("%d [%p] %m%n");

    OstreamAppender *pOsAppender = new OstreamAppender("pOsAppender", &cout);
    pOsAppender->setLayout(pLayout1);

    RollingFileAppender *pRollAppender = new RollingFileAppender("pRollAppender",
                                                                 "roll.log",
                                                                 4096,
                                                                 5);
    pRollAppender->setLayout(pLayout2);

    _myCategory.setPriority(Priority::DEBUG);
    _myCategory.addAppender(pOsAppender);
    _myCategory.addAppender(pRollAppender);
}
MyLogger::~MyLogger()
{
    cout << "~MyLogger()" << endl;
    Category::shutdown();
}

void MyLogger::error(const char *msg) const
{
    _myCategory.error(msg);
}
void MyLogger::warn(const char *msg) const
{
    _myCategory.warn(msg);
}
void MyLogger::info(const char *msg) const
{
    _myCategory.info(msg);
}
void MyLogger::debug(const char *msg) const
{
    _myCategory.debug(msg);
}

// 用带参数的宏削除 __FILE__等编译器自带宏的只显示当前所在行号的影响
#define prefix(msg) string("[").append(__FILE__).append(": ").append(__FUNCTION__).\
                    append(": ").append(to_string(__LINE__)).append("] ").append(msg)

int main()
{
    MyLogger &logger = MyLogger::getInstance();
    logger.error(prefix("WOW ERROR GEN").c_str());
    logger.warn(prefix("Hey this is WARN").c_str());
    logger.info(prefix("Normal info").c_str());
    logger.debug(prefix("just debug").c_str());

    return 0;
}