#include"RosCore.h"//引入头文件RosCore.h
#include<iostream>
#include<stdexcept>
#include"Message.h"

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
//注册订阅者，带异常校验
void RosCore::regSubscriber(std::string topic,Subscriber*sub)
{
    if(sub->getNodeName().empty())
    {
        throw std::runtime_error("ERROR:节点名称不能为空");
    }

   if(topic.empty())
   {
    throw std::runtime_error("ERROR:话题名称不能为空");
   }
   auto& vec=subMap[topic];//获取指定主题的订阅者列表
   for(auto s:vec)//遍历订阅者列表，检查是否已经注册过该订阅者
   {
    if(s==sub)
    {
        throw std::runtime_error("ERROR:订阅者已注册该话题，重复注册");
    }
   }
    vec.push_back(sub);//如果没有注册过，则将订阅者添加到列表中
    std::cout<<"[INFO]Subcriber register success,topic:"<<topic<<std::endl;
}
void RosCore::unregSubscriber(std::string topic,Subscriber*sub)
{
    auto it=subMap.find(topic);//查找指定主题的订阅者列表
    if(it!=subMap.end())//如果找到了该主题
    {
        auto& vec=it->second;//获取订阅者列表
       for(auto iter=vec.begin();iter!=vec.end();)//遍历订阅者列表，查找指定的订阅者
       {
        if(*iter==sub)//如果找到了该订阅者
        {
           iter=vec.erase(iter);//从列表中移除该订阅者
            std::cout<<"[INFO]Subcriber unregister success,topic:"<<topic<<std::endl;
        }
        else//如果没有找到该订阅者，继续遍历
        {
            ++iter;
        }
       }
    }
}
//发布消息，带异常校验
void RosCore::regPublisher(std::string topic,Publisher*pub)
{  
    if(pub->getNodeName().empty())
    {
        throw std::runtime_error("ERROR:节点名称不能为空");
    }                                 
    if(topic.empty())
    {
        throw std::runtime_error("ERROR:话题名称不能为空");
    }
    auto& vec=pubMap[topic];
    for(auto p:vec)//遍历发布者列表，检查是否已经注册过该发布者
    {
        if(p==pub)
        {
            throw std::runtime_error("ERROR:发布者已注册该话题，重复注册");
        }
    }   
    vec.push_back(pub);//如果没有注册过，则将发布者添加到列表中
    std::cout<<"[INFO]Publisher register success,topic:"<<topic<<std::endl;
}
void RosCore::unregPublisher(std::string topic,Publisher*pub)
{
    auto it=pubMap.find(topic);//查找指定主题的发布者列表
    if(it!=pubMap.end())//如果找到了该主题
    {
        auto& vec=it->second;//获取发布者列表
        for(auto iter=vec.begin();iter!=vec.end();)//遍历发布者列表，查找指定的发布者
        {
            if(*iter==pub)//如果找到了该发布者
            {
                iter=vec.erase(iter);//从列表中移除该发布者
                std::cout<<"[INFO]Publisher unregister success,topic:"<<topic<<std::endl;
            }
            else//如果没有找到该发布者，继续遍历
            {
                ++iter;
            }
        }
    }
}
//发布消息，带异常校验:空消息，不存在主题，主题没有订阅者
void RosCore::publish(std::string topic,Message msg)
{

    if(msg.getData().empty())//检查消息内容是否为空
    {
        throw std::runtime_error("ERROR:消息内容不能为空");
    }
    auto it=subMap.find(topic);//查找指定主题的订阅者列表
    if(it==subMap.end()||it->second.empty())//如果没有找到该主题或主题没有订阅者
    {
        throw std::runtime_error(std::string("ERROR:向不存在订阅者的话题转发消息：")+topic);
    }
    for(auto sub:it->second)//遍历订阅者列表，向每个订阅者发送消息
    {
        sub->callback(msg);
    }
    std::cout<<"[INFO]RosCore:publish msg to topic:"<<topic<<std::endl;
}
//全局关闭：遍历所有节点，调用shutdown()
void RosCore::shutdownAll()
{
    //关闭全部订阅者
    for (auto& entry : subMap) 
    {
        for (auto sub : entry.second)
         {
            sub->shutdown();
            std::cout << "[INFO]Subscriber closed,topic:" <<entry.first<<std::endl;
        }
    }
//关闭全部发布者
    for (auto& entry : pubMap) 
    {
        for (auto pub : entry.second) 
        {
            pub->shutdown();
            std::cout << "[INFO]Publisher closed,topic:" <<entry.first<<std::endl;
        }
    }
    std::cout << "[INFO]RosCore:All nodes closed successfully." << std::endl;
}