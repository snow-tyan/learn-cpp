#include "Acceptor.hh"
#include "TCPConnection.hh"

#include <iostream>
using namespace std;
using namespace wd;

int main()
{
    Acceptor acceptor("172.25.40.81", 8888);
    acceptor.ready();

    TCPConnection tcp(acceptor.accept());
    cout << tcp.toString() << " has connected!" << endl;
    tcp.send("Welcome to server.");
    cout << ">> client data:" << endl;
    cout << tcp.recv() << endl;
    return 0;
}