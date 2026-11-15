# ros_sim 简易ROS发布订阅模拟器
## 项目说明
模拟ROS发布订阅通信模型，包含RosCore、Publisher、Subscriber、Node、Message。
程序运行后终端打印【发布】【接收】日志，直观展示消息流转过程。
支持分级日志输出、自定义异常捕获，日志同时打印到CMD控制台并自动保存至 `log.txt`。
扩展功能：带时间戳的SensorMessage传感器消息、单例设计、消息只读保护。

##环境要求
-C++11及以上
-g++编译器

##文件结构
src/
|-----main.cpp
|-----RosCore.h/RosCore.cpp
|-----Publisher.h/Publisher.cpp
|-----Subcriber.h/Subcriber.cpp
|-----Message.h
|-----Node.h

## 编译命令
进入src目录，终端执行：
```bash
g++ main.cpp RosCore.cpp Publisher.cpp Subcriber.cpp -o main -std=c++11

##运行
./main.exe

##程序功能
1.订阅者先注册，发布者后注册，同一话题 /chatter
2. 发布消息自动携带时间戳
3. RosCore 负责消息分发，回调给订阅者 callback
4. 程序结束自动注销节点、执行 shutdown
5. 控制台打印注册、发布、接收、注销日志

##运行示例输出
[INFO]Subscriber register success,topic:/chatter
[INFO]Subscriber[sub1]init,topic:/chatter
[INFO]Publisher[publish]init,topic:/chatter
[INFO]Publisher register success,topic:/chatter
[Receive]topic:/chatter content:hello ros_sim time:2026-11-15 12:59:29
[Publish]topic:/chatter content:hello ros_sim time:2026-11-15 12:59:29
[INFO]Publisher[publish]shutdown,topic:/chatter
[INFO]Publisher unregister success,topic:/chatter
[INFO]Subscriber[sub1]shutdown,topic:/chatter
[INFO]Subscriber unregister success,topic:/chatter




