//封装log4cpp + 可变参数

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

    template <class... Args>
    void error(Args... args) const;

    template <class... Args>
    void warn(Args... args) const;

    template <class... Args>
    void info(Args... args) const;

    template <class... Args>
    void debug(Args... args) const;

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
    // %d{%Y%m%d 年月日 %H:%M%S 时分秒 %l 毫秒}
    // %p 优先级 %m 消息 %n 换行符
    pLayout1->setConversionPattern("%d{%m-%d %H:%M} [%-5p] %m%n");
    PatternLayout *pLayout2 = new PatternLayout();
    pLayout2->setConversionPattern("%d{%m-%d %H:%M} [%-5p] %m%n");

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

template <class... Args>
void MyLogger::error(Args... args) const
{
    _myCategory.error(args...); // category本身就可以接受可变参数，所以可以直接交给它
}
template <class... Args>
void MyLogger::warn(Args... args) const
{
    _myCategory.warn(args...);
}
template <class... Args>
void MyLogger::info(Args... args) const
{
    _myCategory.info(args...);
}
template <class... Args>
void MyLogger::debug(Args... args) const
{
    _myCategory.debug(args...);
}

// 用带参数的宏削除 __FILE__等编译器自带宏的只显示当前所在行号的影响
#define prefix(msg) string("{").append(__FILE__).append(": ").append(__FUNCTION__).append(": ").append(to_string(__LINE__)).append("} ").append(msg)

#define LogError(msg, ...) MyLogger::getInstance().error(prefix(msg).c_str(), ##__VA_ARGS__) //加两个##号，可以把无参的情况前面逗号删除
#define LogWarn(msg, ...) MyLogger::getInstance().warn(prefix(msg).c_str(), ##__VA_ARGS__)
#define LogInfo(msg, ...) MyLogger::getInstance().info(prefix(msg).c_str(), ##__VA_ARGS__)
#define LogDebug(msg, ...) MyLogger::getInstance().debug(prefix(msg).c_str(), ##__VA_ARGS__)

void test0() //单例
{
    MyLogger &logger = MyLogger::getInstance();
    logger.error(prefix("WOW ERROR GEN").c_str());
    logger.warn(prefix("Hey this is WARN").c_str());
    logger.info(prefix("Normal info").c_str());
    logger.debug(prefix("just debug").c_str());
}

void test1() //封装，使之更好用
{
    LogError("Wow, Hello, error msg");
    LogInfo("Hi Info");
}

void test2() //添加可变参数，但只能直接传给logger
{
    MyLogger &logger = MyLogger::getInstance();
    int number = 10;
    logger.error("This is a error msg, args = %d", 10);
}

void test3() //可变参数 + prefix + 封装
{
    float x = 3.1415926;
    LogError("Wow, Hello, error msg, args = %f", x);
    LogInfo("Hi Info, %5.2f", x);
}
int main()
{
    // test0();
    // test1();
    // test2();
    test3();
    return 0;
}