#include"Publisher.h"
#include"RosCore.h"
#include<iostream>
using namespace std; 
void Publisher::init()
{
    cout<<"[INFO]Publisher[]"<<getNodeName()<<"]init,topic:"<<topicName<<endl;
    RosCore*core=RosCore::getInstance();//调用单例的静态方法，拿到唯一的RosCore指针
    core->regPublisher(topicName,this);//把当前发布者注册到RosCore,由它做消息分发

}
void Publisher::shutdown()
{
    cout<<"[INFO]Publisher["<<getNodeName()<<"]shutdown,topic:"<<topicName<<endl;
    RosCore*core=RosCore::getInstance();//调用单例的静态方法，拿到唯一的RosCore指针
    core->unregPublisher(topicName,this);//把当前发布者从RosCore注销,由它做消息分发
}
void Publisher::publish(Message msg)
{
    RosCore*core=RosCore::getInstance();//调用单例的静态方法，拿到唯一的RosCore指针
    core->publish(topicName,msg);//把消息和当前发布者的话题交给RosCore,由它做消息分发
    cout<<"[Publish]topic:"<<topicName<<"content:"<<msg.getData()<<"time:"<<msg.getTimeStamp()<<endl;
}