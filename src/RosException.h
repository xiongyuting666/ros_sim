#ifndef ROSEXCEPTION_H
#define ROSEXCEPTION_H
#include<exception>
#include<string>
//节点异常：节点名非法、节点重复注册等
class NodeException:public std::exception
{
public:
    explicit NodeException(const std::string& msg):errMsg(msg){}
    const char* what() const noexcept override
    {
        return errMsg.c_str();
    }
private:
    std::string errMsg;
};
//话题异常：话题名非法、向无订阅者话题转发等
class TopicException:public std::exception
{
public:
    explicit TopicException(const std::string& msg):errMsg(msg){}
    const char* what() const noexcept override
    {
        return errMsg.c_str();
    }
private:
    std::string errMsg;
};
//消息异常：消息内容为空、消息重复投递等
class MessageException:public std::exception
{
public:
    explicit MessageException(const std::string& msg):errMsg(msg){}
    const char* what() const noexcept override
    {
        return errMsg.c_str();
    }
private:
    std::string errMsg;
};
//服务异常：服务名非法、调用不存在的服务等
class ServiceException:public std::exception
{
public:
    explicit ServiceException(const std::string& msg):errMsg(msg){}
    const char* what() const noexcept override
    {
        return errMsg.c_str();
    }
private:
    std::string errMsg;
};
#endif