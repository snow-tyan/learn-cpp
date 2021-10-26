//丰富simple String
//由于还没看过allocate，所以+=的实现是每次都会去重新申请空间再copy

#include <string.h>
// #include <assert.h>
#include <iostream>
#include <vector>

using namespace std;

#define MOVE // 移动语义

class MyString
{
public:
    MyString();
    MyString(const char *);
    MyString(const MyString &);
    MyString(const size_t, const char);
    MyString &operator=(const char *);
    MyString &operator=(const MyString &);
#ifdef MOVE
    MyString(MyString &&);
    MyString &operator=(MyString &&);
#endif
    ~MyString();

    size_t size() const;
    size_t length() const;
    const char *c_str();

    //成员函数形式重载
    MyString &operator+=(const char *);
    MyString &operator+=(const MyString &);
    char &operator[](size_t idx);             //非const对象调用[]
    const char &operator[](size_t idx) const; // const对象调用operator[]

    //友元形式重载
    friend ostream &operator<<(ostream &os, const MyString &rhs);
    friend istream &operator>>(istream &is, MyString &rhs);
    friend bool operator==(const MyString &lhs, const MyString &rhs);
    friend bool operator<(const MyString &lhs, const MyString &rhs);
    friend bool operator>(const MyString &lhs, const MyString &rhs);

private:
    char *_pstr;
    size_t _length;
};

//普通函数形式重载
MyString operator+(const MyString &lhs, const MyString &rhs)
{
    MyString newString = lhs;
    newString += rhs;
    return newString;
}
MyString operator+(const MyString &lhs, const char *pstr)
{
    MyString newString = lhs;
    newString += pstr;
    return newString;
}
MyString operator+(const char *pstr, const MyString &rhs)
{
    MyString newString = pstr;
    newString += rhs;
    return newString;
}

MyString::MyString()
    : _pstr(new char()), _length(0)
{
    cout << "MyString()" << endl;
    strcpy(_pstr, "");
};
MyString::MyString(const char *pstr)
    : _pstr(new char[strlen(pstr) + 1]()), _length(strlen(pstr))
{
    cout << "MyString(const char *)" << endl;
    strcpy(_pstr, pstr);
}
MyString::MyString(const MyString &rhs)
    : _pstr(new char[strlen(rhs._pstr) + 1]()), _length(strlen(rhs._pstr))
{
    cout << "MyString(const MyString &)" << endl;
    strcpy(_pstr, rhs._pstr);
}
MyString::MyString(const size_t len, const char ch)
    : _pstr(new char[len + 1]()), _length(len)
{
    cout << "MyString(const size_t, const char *" << endl;
    for (size_t i = 0; i != len; ++i)
        _pstr[i] = ch;
    _pstr[len] = '\0';
}
MyString::~MyString()
{
    cout << "~MyString()" << endl;
    if (_pstr)
        delete _pstr;
}

MyString &MyString::operator=(const char *pstr)
{
    if (_pstr)
        delete _pstr;
    _length = strlen(pstr);
    _pstr = new char[_length + 1]();
    strcpy(_pstr, pstr);
    return *this;
}
MyString &MyString::operator=(const MyString &rhs)
{
    cout << "MyString operator=(const MyString &)" << endl;
    if (this != &rhs)
    {
        if (_pstr)
            delete[] _pstr;
        _length = strlen(rhs._pstr);
        _pstr = new char[_length + 1]();
        strcpy(_pstr, rhs._pstr);
    }
    return *this;
}

#ifdef MOVE
MyString::MyString(MyString &&rhs)
    : _pstr(rhs._pstr) //浅拷贝
{
    cout << "MyString(MyString &&)" << endl;
    rhs._pstr = nullptr; //临时对象置空，防止被析构回收掉该处资源
}
MyString &MyString::operator=(MyString &&rhs)
{
    cout << "MyString operator=(MyString &&)" << endl;
    // 无需考虑自复制吧，错，还有这种情况 s1 = std::move(s1)
    if (this == &rhs)
        return *this;
    if (_pstr)
        delete[] _pstr;
    _pstr = rhs._pstr;
    rhs._pstr = nullptr;
    return *this;
}
#endif

size_t MyString::size() const
{
    return _length;
}
size_t MyString::length() const
{
    return _length;
}
const char *MyString::c_str()
{
    return _pstr;
}

MyString &MyString::operator+=(const char *pstr)
{
    if (pstr) //判断右操作数，左操作数this至少是个空串不可能为空
    {
        char *pold = _pstr;
        _length += strlen(pstr);
        _pstr = new char[_length + 1]();
        strcpy(_pstr, pold);
        strcat(_pstr, pstr);
        delete pold;
    }
    return *this;
}
MyString &MyString::operator+=(const MyString &rhs)
{
    if (rhs._length != 0) //判断空串
    {
        if (this == &rhs)
        {
            MyString self(*this); //复制一下自己
            return *this += self; //再调用一下自己
        }
        _length += rhs._length;
        char *pold = _pstr;
        _pstr = new char[_length + 1]();
        strcpy(_pstr, pold);
        strcat(_pstr, rhs._pstr);
        delete[] pold;
        // else //这里不是很必要，MyString对象即使是空串也有一个字节空间
        //      //，_pstr也指向那个'\0'，不可能为空
        // {
        //     _pstr = new char[_length + 1]();
        //     strcpy(_pstr, rhs._pstr);
        // }
    }
    return *this;
}

char &MyString::operator[](size_t idx)
{
    if (idx < 0 || idx >= _length)
    {
        cout << "越界异常" << endl;
        static char nullchar = '\0';
        return nullchar;
    }
    return _pstr[idx];
}
const char &MyString::operator[](size_t idx) const
{
    cout << "const []" << endl;
    if (idx < 0 || idx >= _length)
    {
        cout << "越界异常" << endl;
        static char nullchar = '\0';
        return nullchar;
    }
    return _pstr[idx];
}

//友元
ostream &operator<<(ostream &os, const MyString &rhs)
{
    return os << rhs._pstr;
}
istream &operator>>(istream &is, MyString &rhs)
{
    is >> rhs._pstr;
    return is;
}
bool operator==(const MyString &lhs, const MyString &rhs)
{
    return strcmp(lhs._pstr, rhs._pstr) == 0;
}
bool operator!=(const MyString &lhs, const MyString &rhs)
{
    return !(lhs == rhs);
}
bool operator<(const MyString &lhs, const MyString &rhs)
{
    return strcmp(lhs._pstr, rhs._pstr) < 0;
}
bool operator>=(const MyString &lhs, const MyString &rhs)
{
    return !(lhs < rhs);
}
bool operator>(const MyString &lhs, const MyString &rhs)
{
    return strcmp(lhs._pstr, rhs._pstr) > 0;
}
bool operator<=(const MyString &lhs, const MyString &rhs)
{
    return !(lhs > rhs);
}

void test0()
{
    MyString s1;
    MyString s2("I am s2");
    MyString s3(s2);
    MyString s4(4, 's');
    cout << s1 << endl;
    cout << s2 << "  s2.size()=" << s2.size() << endl;
    cout << s3 << "  s3.length()=" << s3.length() << endl;
    cout << s4 << s4.c_str() << endl;
    s1 = s4;
    cout << s1 << endl;
    s1 = "I am s1";
    cout << s1 << " s1.length()=" << s1.length() << endl;
    s1 += s2;
    cout << s1 << " s1.length()=" << s1.length() << endl;
    s1 += "I am s3";
    cout << s1 << " s1.length()=" << s1.length() << endl;
    s1 += "";
    cout << s1 << " s1.length()=" << s1.length() << endl;
    cout << s1[0] << endl; // 对非const对象使用[]会默认调用非const operator[]因为无法判断是否会修改值
    const MyString s6("s6");
    cout << s6[1] << endl; // const operator[]
    s1[0] = 'Y';
    cout << s1 << endl;
    MyString s5;
    cin >> s5;
    cout << s5 << endl;
}

void test1()
{
    vector<MyString> strs;
    // 没有移动构造之前，它会调用拷贝构造，深拷贝一份放到strs，然后析构掉自身
    // 有了移动构造，它会调用移动构造，浅拷贝指向临时对象，放入strs，析构自身不会回收资源
    strs.push_back("helloworld");
    cout << strs[0] << endl;

    MyString s1 = "hello";
    s1 = MyString("world");
    MyString s2 = "Changchun";
    //显示将左值转换成右值 std::move
    s1 = std::move(s2); // 转成右值之后，s2就没了...
    cout << s2 << endl;
    cout << "NOT SHOW" << endl; //不会打印出来，因为上一句已经崩了...
}

int main(void)
{
    // test0();
    test1();
    return 0;
}