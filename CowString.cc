// cowString
#include <string.h>
#include <iostream>
using namespace std;

class CowString
{
    //代理模式
    class CharProxy
    {
    public:
        CharProxy(CowString &self, size_t idx)
            : _self(self), _idx(idx)
        {
            // cout << "CharProxy(CowString &, size_t)" << endl;
        }

        //执行写操作
        // CharProxy &operator=(const char &);
        char &operator=(const char &);

        //读操作, 类型转换->char 就不用重载输出流运算符
        operator char() { return _self._pstr[_idx]; }
        // friend ostream &operator<<(ostream &, const CharProxy &);

    private:
        CowString &_self;
        size_t _idx;
    };

public:
    CowString();
    CowString(const char *);
    CowString(const CowString &);            //浅拷贝，引用计数+1
    CowString &operator=(const CowString &); //浅拷贝
    ~CowString();

    const char *c_str() const;
    size_t size() const;
    size_t length() const;

    //下标运算符区分不了读写，这也是写时复制的一个缺点 “草木皆兵”
    const char &operator[](size_t idx) const; // const对象执行下标运算符
    // char &operator[](size_t idx);
    CharProxy operator[](size_t idx); //区分读写操作的下标访问运算符

    friend ostream &operator<<(ostream &os, const CowString &);
    friend istream &operator>>(istream &is, CowString &);
    // friend ostream &operator<<(ostream &, const CharProxy &); //它还必须是CowString的友元

    int refCount() { return *(int *)(_pstr - 4); }

private:
    void initRefCount() { *(int *)(_pstr - 4) = 1; }
    void increaseRefCount() { ++*(int *)(_pstr - 4); }
    void decreaseRefCount() { --*(int *)(_pstr - 4); }
    void release()
    {
        decreaseRefCount();
        if (0 == refCount())
        {
            delete[](_pstr - 4); // delete所申请的堆空间内容
            cout << ">> heap release" << endl;
        }
    }

private:
    //引用计数放在字符串前4个字节处
    char *_pstr; //指向字符串首地址
};

CowString::CowString()
    : _pstr(new char[4 + 1]() + 4) //后移4个字节才是字符串开头
{
    cout << "CowString()" << endl;
    initRefCount();
}
CowString::CowString(const char *pstr)
    : _pstr(new char[4 + strlen(pstr) + 1]() + 4)
{
    cout << "CowString(const char *)" << endl;
    initRefCount();
    strcpy(_pstr, pstr);
}
CowString::CowString(const CowString &rhs)
    : _pstr(rhs._pstr) //浅拷贝
{
    cout << "CowString(const CowString &)" << endl;
    increaseRefCount();
}
CowString &CowString::operator=(const CowString &rhs)
{
    if (this != &rhs)
    {
        release();
        _pstr = rhs._pstr; //浅拷贝
        increaseRefCount();
    }
    return *this;
}
CowString::~CowString() { release(); }

const char *CowString::c_str() const { return _pstr; }
size_t CowString::size() const { return strlen(_pstr); }
size_t CowString::length() const { return strlen(_pstr); }

ostream &operator<<(ostream &os, const CowString &rhs) { return os << rhs._pstr; }
istream &operator>>(istream &is, CowString &rhs) { return is >> rhs._pstr; }

const char &CowString::operator[](size_t idx) const
{
    if (idx < 0 || idx >= length())
    {
        cout << "下标越界" << endl;
        static char nullchar = '\0';
        return nullchar;
    }
    return _pstr[idx];
}
#if 0
char &CowString::operator[](size_t idx)
{
    if (idx < 0 || idx >= length())
    {
        cout << "下标越界" << endl;
        static char nullchar = '\0';
        return nullchar;
    }
    //引用计数大于1才会复制一份，否则原地修改
    if (refCount() > 1)
    {
        decreaseRefCount();
        char *ptemp = _pstr;
        _pstr = new char[4 + strlen(_pstr) + 1]() + 4;
        strcpy(_pstr, ptemp);
        initRefCount();
    }
    return _pstr[idx];
}
#endif
#if 0
ostream &operator<<(ostream &os, const CowString::CharProxy &rhs)
{
    return os << rhs._self._pstr;
}
#endif
CowString::CharProxy CowString::operator[](size_t idx)
{
    return CharProxy(*this, idx);
}
#if 0
CowString::CharProxy &CowString::CharProxy::operator=(const char &ch)
{
    //引用计数大于1才会复制一份，否则原地修改
    if (_idx > 0 && _idx <= _self.length())
    {
        if (_self.refCount() > 1)
        {
            _self.decreaseRefCount();
            char *ptemp = _self._pstr;
            _self._pstr = new char[4 + strlen(_self._pstr) + 1]() + 4;
            strcpy(_self._pstr, ptemp);
            _self._pstr[_idx] = ch;
            _self.initRefCount();
        }
    }
    return *this;
}
#else
char &CowString::CharProxy::operator=(const char &ch)
{
    if (_idx < 0 || _idx >= _self.length())
    {
        cout << "下标越界" << endl;
        static char nullchar = '\0';
        return nullchar;
    }
    //引用计数大于1才会复制一份，否则原地修改
    if (_self.refCount() > 1)
    {
        _self.decreaseRefCount();
        char *ptemp = _self._pstr;
        _self._pstr = new char[4 + strlen(_self._pstr) + 1]() + 4;
        strcpy(_self._pstr, ptemp);
        _self._pstr[_idx] = ch;
        _self.initRefCount();
    }
    return _self._pstr[_idx];
}
#endif

int main()
{
    CowString s1("hello,world");
    CowString s2(s1);
    CowString s3(s2);
    cout << "s1 = " << s1 << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    printf("s1's addr= %p\n", s1.c_str());
    printf("s2's addr= %p\n", s2.c_str());
    printf("s3's addr= %p\n", s3.c_str());
    cout << "s1'ref = " << s1.refCount() << endl;
    cout << "s2'ref = " << s2.refCount() << endl;
    cout << "s3'ref = " << s3.refCount() << endl;

    cout << "读下标" << endl;
    // const char ch = s3[0];
    cout << "s3[0] = " << s3[0] << endl;
    cout << "s3 = " << s3 << endl;
    printf("s3's addr= %p\n", s3.c_str());
    cout << "s3'ref = " << s3.refCount() << endl;

    cout << "修改下标" << endl;
    s3[0] = 'D';
    cout << "s3[0] = " << s3[0] << endl;
    cout << "s3 = " << s3 << endl;
    printf("s3's addr= %p\n", s3.c_str());
    cout << "s3'ref = " << s3.refCount() << endl;

    return 0;
}