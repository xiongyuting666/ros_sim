#ifndef ROS_CORE_H
#define ROS_CORE_H
#include <string>
#include <vector>
#include<map>
#include"Subscriber.h"
#include"Message.h"


class RosCore
{ 
    private:
    static RosCore* instance; //静态指针，保存唯一的 RosCore 实例（单例模式）
    RosCore(){}; // 私有构造函数，外面不能new Roscore,保证单例模式，如果想，则需要用下面的接口
    std::map<std::string, std::vector<Subscriber*>> topicMap; // 存储主题与订阅者的映射关系，键为主题名称，值为订阅者指针的向量
    public:
    static RosCore* getInstance(); // 获取单例实例的唯一接口
    void subscribe(std::string topic, Subscriber* sub); // 订阅函数，把订阅者添加到指定主题的订阅者列表中
    void publish(std::string topic, Message msg); // 发布函数，向指定主题的所有订阅者发送消息
};

#endif 