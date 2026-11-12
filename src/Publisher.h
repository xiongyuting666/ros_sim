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
    void run() override;// 重写Node类的纯虚函数run()，实现发布者的运行逻辑
    void publish(Message msg);// 发布消息的方法，接受一个Message对象作为参数
};

#endif 