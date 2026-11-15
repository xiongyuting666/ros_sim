#ifndef ROS_CORE_H
#define ROS_CORE_H
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include"Publisher.h"
#include"Subscriber.h"
#include"Message.h"


struct MsgItem
{
    std::string topic; // 消息所属的主题
    std::string content; // 消息的内容
    std::string timeStamp; // 消息的时间戳
};


class RosCore
{ 
    private:
    static RosCore* instance; //静态指针，保存唯一的 RosCore 实例（单例模式）
    RosCore(){}; // 私有构造函数，外面不能new Roscore,保证单例模式，如果想，则需要用下面的接口
    
    //两个map容器（映射表），分别存储主题与订阅者、发布者的映射关系
    std::map<std::string, std::vector<Subscriber*>> subMap; // 存储主题与订阅者的映射关系，键为主题名称，值为订阅者指针的向量
    std::map<std::string, std::vector<Publisher*>> pubMap; // 存储主题与发布者的映射关系，键为主题名称，值为发布者指针的向量
    
    public:
    static RosCore* getInstance(); // 获取单例实例的唯一接口
    void regSubscriber(std::string topic, Subscriber* sub); // 注册订阅者函数，把订阅者添加到指定主题的订阅者列表中
    void unregSubscriber(std::string topic, Subscriber* sub); // 注销订阅者
    void regPublisher(std::string topic, Publisher* pub); // 注册发布者函数，把发布者添加到指定主题的发布者列表中
    void unregPublisher(std::string topic, Publisher* pub); // 注销发布者
    void publish(std::string topic, Message msg); // 发布函数，向指定主题的所有订阅者发送消息
    void shutdownAll(); // 关闭函数，清理资源，注销所有订阅者和发布者
};

#endif 