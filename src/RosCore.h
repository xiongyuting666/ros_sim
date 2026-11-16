#ifndef ROS_CORE_H
#define ROS_CORE_H
#include <string>
#include <vector>
#include <map>
#include "Publisher.h"
#include "Subscriber.h"
#include "Message.h"
#include "RosException.h"
#include "Log.h"
#include "Service.h"

class RosCore
{
private:
    static RosCore* instance; //静态指针，保存唯一的 RosCore 实例（单例模式）
    RosCore() {}; // 私有构造函数，外部不能 new RosCore，保证单例模式

    //两个map容器（映射表），分别存储主题与订阅者、发布者的映射关系
    std::map<std::string, std::vector<Subscriber*>> subMap; // 主题 -> 订阅者列表
    std::map<std::string, std::vector<Publisher*>> pubMap;  // 主题 -> 发布者列表
    std::map<std::string, Service*> serviceMap;          // 服务名 -> 服务对象

public:
    static RosCore* getInstance(); // 获取单例实例的唯一接口
    void regSubscriber(std::string topic, Subscriber* sub);   // 注册订阅者
    void unregSubscriber(std::string topic, Subscriber* sub); // 注销订阅者
    void regPublisher(std::string topic, Publisher* pub);     // 注册发布者
    void unregPublisher(std::string topic, Publisher* pub);   // 注销发布者
    void publish(std::string topic, Message msg);             // 向指定主题的所有订阅者发送消息
    void regService(Service* service);                        // 注册服务端
    ServiceResponse callService(const std::string& name, const ServiceRequest& req); // 客户端同步调用服务
    void shutdownAll();                                       // 全局关闭，注销并关闭所有节点
};
#endif