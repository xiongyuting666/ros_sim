#ifndef NODE_H
#define NODE_H
#include <string>
class Node {
    private://private 修饰符表示后续它的子类(Publisher, Subscriber)不能直接访问它的成员变量nodeName
        std::string nodeName; // 节点名称
    public:
        Node(std::string name) : nodeName(std::move(name)) {} // 有参构造函数，初始化节点名称
        //节点名称只读接口，提供给子类访问节点名称的方式
        std::string getNodeName() const
         {
             return nodeName; 
         } 
        virtual void init() = 0; //节点初始化函数,初始化的纯虚接口
        virtual void shutdown() = 0; //节点关闭函数,关闭的纯虚接口
        virtual ~Node()=default; // 虚析构函数，确保子类对象被正确销毁
};  

#endif 