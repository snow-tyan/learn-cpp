#include <iostream>
#include <regex>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

void test0()
{
    XMLDocument doc;
    doc.LoadFile("36ke.xml");
    if (doc.ErrorID())
    {
        cout << "LoadFile Error" << endl;
    }

    XMLElement *itemNode = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    XMLElement *titleNode = itemNode->FirstChildElement("title");
    const char *title = titleNode->GetText();
    cout << "title=" << title << endl;

    XMLElement *linkNode = itemNode->FirstChildElement("link");
    const char *link = linkNode->GetText();
    cout << "link=" << link << endl;

    XMLElement *descriptionNode = itemNode->FirstChildElement("description");
    const char *description = descriptionNode->GetText();

    regex re("<.+?>");
    regex re2("&nbsp;");
    string reDescription = regex_replace(regex_replace(description, re, ""), re2, "");
    cout << "description=" << reDescription << endl;
}

int main()
{
    test0();

    return 0;
}