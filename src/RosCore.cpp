#include"RosCore.h"//引入头文件RosCore.h
#include<iostream>
RosCore*RosCore::instance=nullptr;//初始化instance静态指针，nullper表示初始化为空，还未创建RosCore实例
//且静态指针不能在类里初始化，必须在类外
RosCore*RosCore::getInstance()//呼应类里获取单例的唯一接口，从而可以new RosCore
{                             
    if(instance==nullptr)//判断有没有创建过实例
    {                    //如果为空，new RosCore()创建唯一的对象，并把地址赋给instance
        instance=new RosCore();
    }
    return instance;//不为空则返回已经存在的实例
}
void RosCore::subscribe(std::string topic,Subscriber*sub)
{
    topicMap[topic].push_back(sub);//把订阅者加入到了这个话题中，RosCore记住谁订阅了这个话题
}
void RosCore::publish(std::string topic,Message msg)
{
    if(topicMap.find(topic)!=topicMap.end())//检查这个话题是否存在订阅者
    //find：去map里找这个话题；！= end:表示找到了，有人订阅
    {
        for(auto sub:topicMap[topic])//遍历这个话题下每一个订阅者指针
        {
            sub->callback(msg);
        }
        //挨个调用订阅者的回调函数，把消息传给订阅者
    }
}
//RosCore的工作：订阅的时候，它负责登记好【话题-订阅者】；发布消息的时候，RosCore找到订阅该主题的每个订阅者，挨个发送消息