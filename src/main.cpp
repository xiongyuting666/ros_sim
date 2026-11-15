#include"Publisher.h"
#include"Subscriber.h"
#include"Message.h"
#include"RosCore.h"
#include<iostream>
#include<windows.h>
using namespace std;
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    try
    {
    //创建发布者，订阅者
    Publisher pub("pub1","/chatter");
    Subscriber sub("sub1","/chatter");
    //启动节点
    sub.init();//先订阅再发消息
    pub.init();
    //构造消息，发布
    Message msg("hello ros_sim");
    pub.publish(msg);
    //关闭节点
    pub.shutdown();
    sub.shutdown();
    }
    catch(const runtime_error& e)
    {
        cout<<e.what()<<endl;
    }
return 0;
}