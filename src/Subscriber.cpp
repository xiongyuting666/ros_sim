#include"Subscriber.h"
#include"RosCore.h"
#include<iostream>
using namespace std;
void Subscriber::callback(Message msg)
{
    cout<<"[Receive]topic:"<<topicName<<"content:"<<msg.getData()<<"time:"<<msg.getTimeStamp()<<endl;
}
void Subscriber::init()
{
    RosCore*core=RosCore::getInstance();
    core->subscribe(topicName,this);
    core->regSubscriber(topicName,this);
    cout<<"[INFO]Subscriber["<<getNodeName()<<"]init,topic:"<<topicName<<endl;
}
void Subscriber::shutdown()
{
    RosCore*core=RosCore::getInstance();
    core->unregSubscriber(topicName,this);
    cout<<"[INFO]Subscriber["<<getNodeName()<<"]shutdown,topic:"<<topicName<<endl;
}