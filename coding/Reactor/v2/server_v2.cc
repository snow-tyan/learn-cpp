#include "Acceptor.hh"
#include "TCPConnection.hh"
#include "EventLoop.hh"

#include <iostream>
#include <string>
using namespace std;
using namespace wd;

// 普通函数版的回调函数
void onConnection(const TCPConnectionPtr &conn)
{
    cout << conn->toString() << " has connected" << endl;
    conn->send("welcome to server");
}

void onMessage(const TCPConnectionPtr &conn){
    cout << "gets from client: " << conn->recv();
    /** 
     * 业务逻辑处理
     * decode
     * compute
     * encode
     * **/   
    conn->send("wo hen shuai");
}

void onClose(const TCPConnectionPtr &conn){
    cout << ">> " << conn->toString() << " has disconnected" << endl;
}

int main()
{
    Acceptor acceptor("172.25.40.81", 8888);
    acceptor.ready();

    EventLoop event(acceptor);
    event.setConnectionCallback(onConnection);
    event.setMessageCallback(onMessage);
    event.setCloseCallback(onClose);

    event.loop();
    return 0;
}