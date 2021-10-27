// 智能指针 自定义删除器

#include <iostream>
#include <memory>
#include <string>

using namespace std;

struct FileDeleter
{
    void operator()(FILE *fp)
    {
        if (fp)
        {
            fclose(fp);
            cout << ">> File Close" << endl;
        }
    }
};

void test0()
{
    FILE *fp = fopen("test.txt", "a+");
    unique_ptr<FILE, FileDeleter> up(fp);
    string msg = "test0()\n";
    fwrite(msg.c_str(), 1, msg.size(), up.get());
}

void test1()
{
    FILE *fp = fopen("test.txt", "a+");
    shared_ptr<FILE> sp(fp, FileDeleter());
    string msg = "test1()\n";
    fwrite(msg.c_str(), 1, msg.size(), sp.get());
}

int main()
{
    test0();
    test1();

    return 0;
}