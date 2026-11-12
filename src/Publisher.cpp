#include"Publisher.h"
#include"RosCore.h"
#include<iostream>
using namespace std; 
void Publisher::publish(Message msg)
{
    RosCore*core=RosCore::getInstance();//调用单例的静态方法，拿到唯一的RosCore指针
    core->publish(topicName,msg);//把消息和当前发布者的话题交给RosCore,由它做消息分发
}
void Publisher::run()
{
    cout<<"Publisher"<<nodeName<<"is running,topic:"<<topicName<<endl;
//启动节点后打印提示，告诉我们这个发布节点已启动，对应的话题是什么
}
