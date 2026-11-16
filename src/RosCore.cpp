#include"RosCore.h"//引入头文件RobCore.h
#include<iostream>
#include<stdexcept>
#include"Message.h"
#include"RosException.h"
#include"Log.h"

RosCore*RosCore::instance=nullptr;//初始化instance静态指针，nullptr表示初始化为空，还未创建RosCore实例
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
        throw NodeException("ERROR:节点名称不能为空");
    }
    if(topic.empty())
    {
        throw TopicException("ERROR:话题名称不能为空");
    }
    auto& vec=subMap[topic];//获取指定主题的订阅者列表
    for(auto s:vec)//遍历订阅者列表，检查是否已经注册过该订阅者
    {
        if(s==sub)
        {
            throw NodeException("ERROR:订阅者已注册该话题，重复注册");
        }
    }
    vec.push_back(sub);//如果没有注册过，则将订阅者添加到列表中
    LOG_INFO("Subscriber register success,topic:"+topic);
}

void RosCore::unregSubscriber(std::string topic,Subscriber*sub)
{
    auto it=subMap.find(topic);//查找指定主题的订阅者列表
    if(it==subMap.end())//该主题从未被注册过
    {
        LOG_WARN("Unregister subscriber failed,topic not found:"+topic);
        return;
    }
    auto& vec=it->second;//获取订阅者列表
    bool found=false;
    for(auto iter=vec.begin();iter!=vec.end();)//遍历订阅者列表，查找指定的订阅者
    {
        if(*iter==sub)//如果找到了该订阅者
        {
            iter=vec.erase(iter);//从列表中移除该订阅者
            found=true;
            LOG_INFO("Subscriber unregister success,topic:"+topic);
        }
        else//如果没有找到该订阅者，继续遍历
        {
            ++iter;
        }
    }
    if(!found)
    {
        LOG_WARN("Unregister subscriber failed,subscriber not found in topic:"+topic);
    }
}

//注册发布者，带异常校验
void RosCore::regPublisher(std::string topic,Publisher*pub)
{
    if(pub->getNodeName().empty())
    {
        throw NodeException("ERROR:节点名称不能为空");
    }
    if(topic.empty())
    {
        throw TopicException("ERROR:话题名称不能为空");
    }
    auto& vec=pubMap[topic];
    for(auto p:vec)//遍历发布者列表，检查是否已经注册过该发布者
    {
        if(p==pub)
        {
            throw NodeException("ERROR:发布者已注册该话题，重复注册");
        }
    }
    vec.push_back(pub);//如果没有注册过，则将发布者添加到列表中
    LOG_INFO("Publisher register success,topic:"+topic);
}

void RosCore::unregPublisher(std::string topic,Publisher*pub)
{
    auto it=pubMap.find(topic);//查找指定主题的发布者列表
    if(it==pubMap.end())//该主题从未被注册过
    {
        LOG_WARN("Unregister publisher failed,topic not found:"+topic);
        return;
    }
    auto& vec=it->second;//获取发布者列表
    bool found=false;
    for(auto iter=vec.begin();iter!=vec.end();)//遍历发布者列表，查找指定的发布者
    {
        if(*iter==pub)//如果找到了该发布者
        {
            iter=vec.erase(iter);//从列表中移除该发布者
            found=true;
            LOG_INFO("Publisher unregister success,topic:"+topic);
        }
        else//如果没有找到该发布者，继续遍历
        {
            ++iter;
        }
    }
    if(!found)
    {
        LOG_WARN("Unregister publisher failed,publisher not found in topic:"+topic);
    }
}

//发布消息，带异常校验：空消息、不存在主题、主题没有订阅者
void RosCore::publish(std::string topic,Message msg)
{
    if(msg.getData().empty())//检查消息内容是否为空
    {
        throw MessageException("ERROR:消息内容不能为空");
    }
    auto it=subMap.find(topic);//查找指定主题的订阅者列表
    if(it==subMap.end()||it->second.empty())//如果没有找到该主题或主题没有订阅者
    {
        throw TopicException(std::string("ERROR:向不存在订阅者的话题转发消息:")+topic);
    }
    for(auto sub:it->second)//遍历订阅者列表，向每个订阅者发送消息
    {
        sub->callback(msg);
    }
    LOG_INFO("RosCore:publish msg to topic:"+topic);
}

//全局关闭：遍历所有节点，调用shutdown()
void RosCore::shutdownAll()
{
    //先复制一份映射表再遍历：shutdown()内部会注销节点、修改原表，
    //直接遍历原表会导致迭代器失效
    std::map<std::string, std::vector<Subscriber*>> subSnapshot = subMap;
    std::map<std::string, std::vector<Publisher*>> pubSnapshot = pubMap;
    //关闭全部订阅者
    for (auto& entry : subSnapshot)
    {
        for (auto sub : entry.second)
        {
            sub->shutdown();
            LOG_INFO("Subscriber closed,topic:"+entry.first);
        }
    }
    //关闭全部发布者
    for (auto& entry : pubSnapshot)
    {
        for (auto pub : entry.second)
        {
            pub->shutdown();
            LOG_INFO("Publisher closed,topic:"+entry.first);
        }
    }
    subMap.clear();
    pubMap.clear();
    LOG_INFO("RosCore:All nodes closed successfully.");
}
//注册服务端：服务名不能为空，不能重复注册
void RosCore::regService(Service* service)
{
    if(service->getServiceName().empty())
    {
        throw ServiceException("ERROR:服务名称不能为空");
    }
    if(serviceMap.find(service->getServiceName())!=serviceMap.end())
    {
        throw ServiceException("ERROR:服务已注册，重复注册:"+service->getServiceName());
    }
    serviceMap[service->getServiceName()]=service;
    LOG_INFO("Service register success,name:"+service->getServiceName());
}

//客户端同步调用服务：找不到服务直接抛异常，调用成功则阻塞等待响应返回
ServiceResponse RosCore::callService(const std::string& name, const ServiceRequest& req)
{
    auto it=serviceMap.find(name);
    if(it==serviceMap.end())
    {
        throw ServiceException("ERROR:调用了不存在的服务:"+name);
    }
    LOG_INFO("Client call service:"+name);
    return it->second->handle(req);
}

//ServiceClient 通过 RosCore 发起同步请求
ServiceResponse ServiceClient::call(const ServiceRequest& req)
{
    return RosCore::getInstance()->callService(serviceName, req);
}