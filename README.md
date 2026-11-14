# ros_sim 简易ROS发布订阅模拟器
## 项目说明
模拟ROS发布订阅通信模型，包含RosCore、Publisher、Subscriber、Node、Message。
程序运行后终端打印【发布】【接收】日志，直观展示消息流转过程。
支持分级日志输出、自定义异常捕获，日志同时打印到CMD控制台并自动保存至 `log.txt`。
扩展功能：带时间戳的SensorMessage传感器消息、单例设计、消息只读保护。

## 编译
```cmd
g++ src/*.cpp -o main -std=c++11
