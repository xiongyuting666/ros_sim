#ifndef ROSEXCEPTION_H
#define ROSEXCEPTION_H
#include<exception>
#include<string>
//节点异常
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
//话题异常
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
//消息异常
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
#endif