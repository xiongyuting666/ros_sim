#ifndef MESSAGE_H
#define MESSAGE_H
// 头文件保护宏：防止头文件被重复包含、编译报错
#include <string>
// 引入标准库中的字符串类，因为我们消息内容是字符串文本
#include<chrono>
#include<iomanip>
#include<sstream>
class Message { // 定义一个名为 Message 的类，消息载体
            private:
                std::string data; // 私有成员变量，存储消息里的数据
                std::string timeStamp; // 私有成员变量，存储消息的时间戳
                //私有工具函数：内部自动获取当前时间
                std::string getCurrentTime()
                {
                    auto now= std::chrono::system_clock::now(); // 获取当前系统时间点
                    std::time_t t= std::chrono::system_clock::to_time_t(now); // 转换为 time_t 类型
                    std::ostringstream oss; // 创建一个字符串流对象，用于格式化时间输出
                    oss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S"); // 格式化时间为 "年-月-日 时:分:秒" 的形式
                    return oss.str(); // 返回格式化后的时间字符串
                }
            public:
                Message(std::string d) : data(d)
                {
                    timeStamp = getCurrentTime(); // 构造函数，初始化消息数据，并获取当前时间戳
                }
                std::string getData() const
                 { 
                    return data;
                 }
                std::string getTimeStamp() const
                 {
                    return timeStamp;
                 }
                 // 公有成员函数，返回消息数据，const 表示该函数不会修改类的任何数据，getData() 是一个访问器函数，用于获取消息内容
                 // 体现了只能读不能改的要求
              };
#endif
