#ifndef NODE_H
#define NODE_H
#include <string>
class Node {
    protected://protected 修饰符表示后续它的子类(Publisher, Subscriber)可以直接访问它的成员变量nodeName
        std::string nodeName; // 节点名称
    public:
        Node(std::string name) : nodeName(name) {} // 有参构造函数，初始化节点名称
        virtual void run() = 0; //节点启动函数
        // Node是抽象类，不能直接实例化，纯虚函数，子类必须重写run()方法，才能实例化对象
        virtual ~Node()=default; // 虚析构函数，确保子类对象被正确销毁
};  

#endif 