#ifndef SERVICE_H
#define SERVICE_H
#include <string>
#include "Log.h"

//服务请求：客户端发给服务端的数据
struct ServiceRequest
{
    std::string content;
    explicit ServiceRequest(const std::string& c = "") : content(c) {}
};

//服务响应：服务端处理完返回给客户端的数据
struct ServiceResponse
{
    std::string content;
    bool success;
    ServiceResponse(const std::string& c = "", bool ok = true) : content(c), success(ok) {}
};

//服务端：先注册到 RosCore，之后等待客户端同步调用
class Service
{
public:
    //服务处理函数：接收请求，返回响应
    typedef ServiceResponse (*Handler)(const ServiceRequest&);

    Service(const std::string& name, Handler handler)
        : serviceName(name), handler(handler) {}

    std::string getServiceName() const
    {
        return serviceName;
    }

    //同步处理一次请求，处理过程打印日志
    ServiceResponse handle(const ServiceRequest& req) const
    {
        LOG_INFO("Service["+serviceName+"]收到请求:"+req.content);
        ServiceResponse res = handler(req);
        LOG_INFO("Service["+serviceName+"]返回响应:"+res.content);
        return res;
    }

private:
    std::string serviceName; // 服务名称，全局唯一
    Handler handler;         // 具体的业务处理函数
};

//客户端：按服务名称向 RosCore 发起同步请求，阻塞等待响应
class ServiceClient
{
public:
    explicit ServiceClient(const std::string& name) : serviceName(name) {}

    std::string getServiceName() const
    {
        return serviceName;
    }

    //在 RosCore.cpp 中实现，避免头文件循环包含
    ServiceResponse call(const ServiceRequest& req);

private:
    std::string serviceName;
};
#endif