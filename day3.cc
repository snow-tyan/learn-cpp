// #include <log4cpp/Category.hh>
// #include <log4cpp/OstreamAppender.hh>
// #include <log4cpp/FileAppender.hh>
// #include <log4cpp/RollingFileAppender.hh>
// #include <log4cpp/BasicLayout.hh>
// #include <log4cpp/Priority.hh>
// #include <log4cpp/PatternLayout.hh>
#include <iostream>

using namespace std;
// using namespace log4cpp;
/*
typedef enum
{
    EMERG = 0,
    FATAL = 0,
    ALERT = 100,
    CRIT = 200,
    ERROR = 300,
    WARN = 400,
    NOTICE = 500,
    INFO = 600,
    DEBUG = 700,
    NOTSET = 800
} PriorityLevel;
*/

/*
日志记录器 Category
日志过滤器 Priority
格式化器   Layout
输出目的地 Appender

用log4cpp写一个简单的日志的流程
    a. 创建一个Appender，并指定其包含的Layout
    b. 从系统中得到Category的根，将Appender添加到该Category中
    c. 设置Category的优先级
    d. 记录日志
    e. 关闭Category
*/
#if 0
void test0()
{
    PatternLayout *myLayout1 = new PatternLayout();
    myLayout1->setConversionPattern("%d [%p] %m%n");
    PatternLayout *myLayout2 = new PatternLayout();
    myLayout2->setConversionPattern("%d [%p] %m%n");
    PatternLayout *myLayout3 = new PatternLayout();
    myLayout3->setConversionPattern("%d [%p] %m%n");

    OstreamAppender *ostreamAppender = new OstreamAppender("ostreamAppender", &cout);
    ostreamAppender->setLayout(myLayout1);

    FileAppender *fileAppender = new FileAppender("fileAppender", "fileAppender.log");
    fileAppender->setLayout(myLayout2);

    RollingFileAppender *rollAppender = new RollingFileAppender("rollAppender",
                                                                "roll.log",
                                                                1024, //单个文件字节
                                                                5);   //几个分卷
    rollAppender->setLayout(myLayout3);

    Category &root = Category::getRoot();
    root.setPriority(Priority::INFO);
    root.addAppender(ostreamAppender);
    root.addAppender(fileAppender);
    root.addAppender(rollAppender);

    for (int i = 0; i <= 150; ++i)
    {
        root.alert("alert msg");
        root.crit("crit msg");
        root.error("error msg");
        root.warn("warn msg");
        root.notice("notice msg");
        root.info("info msg");
        root.debug("can not be printed");
    }
    Category::shutdown();
}
#endif
class Complex
{
public:
    Complex() = default;
    Complex(int real, int image)
        : _real(real), _image(image) { cout << "Complex(int,int)" << endl; }

    Complex(const Complex &rhs)
        : _real(rhs._real), _image(rhs._image) { cout << "Complex(const Complex &)" << endl; }
    void print()
    {
        cout << _real << " + " << _image << "i" << endl;
    }

    Complex &operator++()
    {
        ++_real;
        ++_image;
        return *this;
    }
    Complex operator++(int)
    {
        Complex temp(*this);
        ++_real;
        ++_image;
        return temp;
    }
    Complex &operator+=(const Complex &rhs)
    {
        _real += rhs._real;
        _image += rhs._image;
        return *this;
    }

    friend Complex operator+(const Complex &lhs, const Complex &rhs);
    friend ostream &operator<<(ostream &os, const Complex &rhs);
    friend istream &operator>>(istream &is, Complex &rhs);
    friend bool operator==(const Complex &lhs, const Complex &rhs);

private:
    int _real;
    int _image;
};

Complex operator+(const Complex &lhs, const Complex &rhs)
{
    return Complex(lhs._real + rhs._real, lhs._image + rhs._image); //打印Complex(int, int)
}
ostream &operator<<(ostream &os, const Complex &rhs)
{
    //考虑打印负数和1和0 的情况，事情就变得复杂起来
    //这里不谈
    return os << rhs._real << " + " << rhs._image << "i";
}
istream &operator>>(istream &is, Complex &rhs)
{
    is >> rhs._real;
    is >> rhs._image;
    return is;
}

bool operator==(const Complex &lhs, const Complex &rhs)
{
    return (lhs._real == rhs._real) && (lhs._image == rhs._image);
}
bool operator!=(const Complex &lhs, const Complex &rhs)
{
    return !(lhs == rhs);
}

int main()
{
    Complex a(1, 2);
    Complex b(2, 3);
    Complex c(a + b);

    c.print();
    cout << a + b << endl;
    // Complex d;
    // cin >> d;
    // cout << d << endl;

    return 0;
}