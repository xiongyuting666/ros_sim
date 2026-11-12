#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "Node.h"
#include "Message.h"

class Subscriber : public Node {// Subscriber类继承自Node类，表示一个订阅者节点
    private:
        std::string topicName; // 订阅的主题
    public:
        Subscriber(std::string name, std::string topic) : Node(name), topicName(topic) {}// 构造函数，先调用父类构造函数，初始化节点名称和主题名称
        void run() override;// 重写Node类的纯虚函数run()，实现订阅者的运行逻辑
        void callback(Message msg);// 回调函数，用于处理接收到的消息
};

#endif 