#include"Subscriber.h"
#include"RosCore.h"
#include<iostream>
using namespace std;
void Subscriber::callback(Message msg)
{
    cout<<"【接收】话题："<<topicName<<"消息内容："<<msg.getData()<<endl;
}
void Subscriber::run()
{
    RosCore*core=RosCore::getInstance();
    core->subscribe(topicName,this);
    cout<<"Subscriber"<<nodeName<<"is running,topic:"<<topicName<<endl;

}