# ROS简易消息仿真系统 用户手册

## 1. 简介
本程序是简化版 ROS 发布-订阅（Publisher-Subscriber）消息通信仿真系统。
程序模拟 ROS 核心调度机制，由 RosCore 统一管理节点与话题映射，实现多个发布者向同一话题发布消息、多个订阅者接收并回调处理的异步通信过程。

程序提供以下功能：
- 基础发布订阅消息转发
- 节点注册、话题订阅、消息回调
- 自定义异常捕获（节点 / 话题 / 消息三类异常）
- INFO / WARN / ERROR 分级日志
- 日志同时输出到控制台并持久化写入 log.txt 文件
- 带时间戳的传感器扩展消息

## 2. 运行环境
- 操作系统：Windows
- 编译器：MinGW-w64（g++），并已将 MinGW 的 bin 目录加入系统环境变量 PATH
- 标准：C++11 及以上
- 依赖：仅使用标准 C++ 库，无第三方依赖

## 3. 源码文件说明
| 文件 | 说明 |
| --- | --- |
| main.cpp | 程序入口，组织节点创建、初始化、消息发布与关闭流程 |
| RosCore.hpp / RosCore.cpp | 全局核心调度器（单例），维护话题-订阅者映射与节点集合 |
| Node.hpp | 抽象节点基类，定义 init()、shutdown() 生命周期接口 |
| Publisher.hpp / Publisher.cpp | 发布者类，绑定话题并向核心发送消息 |
| Subscriber.hpp / Subscriber.cpp | 订阅者类，订阅话题并接收回调处理 |
| Message.hpp | 基础消息类；SensorMessage 传感器扩展消息（温度、湿度、时间戳） |
| exception.hpp | 自定义异常：NodeException / TopicException / MessageException |
| Logger.hpp / Logger.cpp | 分级日志模块（单例），控制台 + 文件双输出 |
| log.txt | 程序运行后自动生成，保存全部 INFO / WARN / ERROR 日志 |

## 4. 编译方法（Windows CMD）

### 4.1 打开命令提示符
按 `Win + R`，输入 `cmd`，回车，打开命令提示符窗口。

### 4.2 进入源码目录
使用 `cd` 命令进入项目源码所在文件夹，例如：
```cmd
cd C:\code\ros_sim
