#include"Subscriber.h"
#include"RosCore.h"
#include"Log.h"

void Subscriber::callback(Message msg)
{
    LOG_INFO("[Receive]subscriber:"+getNodeName()+" topic:"+topicName
             +" content:"+msg.getData()+" time:"+msg.getTimeStamp());
}
void Subscriber::init()
{
    RosCore*core=RosCore::getInstance();
    core->regSubscriber(topicName,this);
    LOG_INFO("Subscriber["+getNodeName()+"]init,topic:"+getTopicName());
}
void Subscriber::shutdown()
{
    RosCore*core=RosCore::getInstance();
    core->unregSubscriber(topicName,this);
    LOG_INFO("Subscriber["+getNodeName()+"]shutdown,topic:"+getTopicName());
}