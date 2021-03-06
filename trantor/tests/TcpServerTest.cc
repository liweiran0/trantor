#include <trantor/net/TcpServer.h>
#include <trantor/utils/Logger.h>
#include <trantor/net/EventLoopThread.h>
#include <string>
#include <iostream>
using namespace trantor;
int main()
{
    LOG_DEBUG<<"test start";
    Logger::setLogLevel(Logger::TRACE);
    EventLoopThread loopThread;
    loopThread.run();
    InetAddress addr(8888);
    TcpServer server(loopThread.getLoop(),addr,"test");
    server.setRecvMessageCallback([](const TcpConnectionPtr &connectionPtr,MsgBuffer *buffer){
        //LOG_DEBUG<<"recv callback!";
        std::cout<<std::string(buffer->peek(),buffer->readableBytes());
        connectionPtr->send(buffer->peek(),buffer->readableBytes());
        buffer->retrieveAll();
        connectionPtr->shutdown();
    });
    server.setConnectionCallback([](const TcpConnectionPtr& connPtr){
        if(connPtr->connected())
        {
            LOG_DEBUG<<"New connection";
        }
        else if(connPtr->disconnected())
        {
            LOG_DEBUG<<"connection disconnected";
        }
    });
    server.setIoLoopNum(3);
    server.start();
    loopThread.wait();
}