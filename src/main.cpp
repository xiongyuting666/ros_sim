#include"Publisher.h"
#include"Subscriber.h"
#include"Message.h"
#include<iostream>
using namespace std;
int main()
{
    //创建发布者，订阅者
    Publisher pub("pub1","/chatter");
    Subscriber sub("sub1","/chatter");
    //启动节点
    sub.run();//先订阅再发消息
    pub.run();
    //构造消息，发布
    Message msg("hello ros_sim");
    pub.publish(msg);
return 0;
}