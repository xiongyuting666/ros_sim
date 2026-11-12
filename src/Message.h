#ifndef MESSAGE_H
#define MESSAGE_H
// 头文件保护宏：防止头文件被重复包含、编译报错
#include <string>
// 引入标准库中的字符串类，因为我们消息内容是字符串文本
class Message { // 定义一个名为 Message 的类，消息载体
            private:
                std::string data; // 私有成员变量，存储消息里的数据
            public:
                Message(std::string d) : data(d) {} 
                // 有参构造函数，初始化消息数据
                std::string getData() const
                 { 
                    return data;
                 }
                 // 公有成员函数，返回消息数据，const 表示该函数不会修改类的任何数据，getData() 是一个访问器函数，用于获取消息内容
                 // 体现了只能读不能改的要求
              };
#endif
