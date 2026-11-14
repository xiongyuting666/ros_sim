#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "Node.h"
#include "Message.h"

class Publisher : public Node {
private:
    std::string topicName; // 发布的主题
        // 这里可以添加Publisher特有的成员变量和方法
public:
    Publisher(std::string name, std::string topic) : Node(name), topicName(topic) {}// 构造函数，先调用父类构造函数，初始化节点名称和主题名称
   //主题只读接口，提供给子类访问主题名称的方式
    std::string getTopicName() const
     {
         return topicName; 
     } 
    void init() override;// 重写Node类的纯虚函数init()，实现发布者的初始化逻辑
    void shutdown() override;// 重写Node类的纯虚函数shutdown()，实现发布者的关闭逻辑
    void publish(Message msg);// 发布消息的方法，接受一个Message对象作为参数
};

#endif 