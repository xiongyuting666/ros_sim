#include"Publisher.h"
#include"RosCore.h"
#include"Log.h"

void Publisher::init()
{
    RosCore*core=RosCore::getInstance();//调用单例的静态方法，拿到唯一的RosCore指针
    core->regPublisher(topicName,this);//把当前发布者注册到RosCore,由它做消息分发
    LOG_INFO("Publisher["+getNodeName()+"]init,topic:"+getTopicName());
}
void Publisher::shutdown()
{
    RosCore*core=RosCore::getInstance();
    core->unregPublisher(topicName,this);//把当前发布者从RosCore注销
    LOG_INFO("Publisher["+getNodeName()+"]shutdown,topic:"+getTopicName());
}
void Publisher::publish(Message msg)
{
    RosCore*core=RosCore::getInstance();
    core->publish(topicName,msg);//把消息和当前发布者的话题交给RosCore,由它做消息分发
    LOG_INFO("[Publish]topic:"+topicName+" content:"+msg.getData()+" time:"+msg.getTimeStamp());
}
//模拟接收传感器上报的字符串信息，创建消息对象（自动生成时间戳）后转发
void Publisher::publish(const std::string& sensorData)
{
    LOG_INFO("Publisher["+getNodeName()+"]模拟接收传感器字符串:"+sensorData);
    Message msg(sensorData);
    publish(msg);
}