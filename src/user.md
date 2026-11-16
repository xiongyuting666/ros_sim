# ROS简易消息仿真系统 用户手册

## 1. 简介
本程序是简化版 ROS 发布-订阅（Publisher-Subscriber）消息通信仿真系统。
程序模拟 ROS 核心调度机制，由 RosCore 统一管理节点与话题映射，实现多个发布者向同一话题发布消息、多个订阅者接收并回调处理的通信过程。

程序提供以下功能：
- 基础发布订阅消息转发、多订阅者广播
- 节点注册、话题订阅、消息回调
- 自定义异常捕获（节点 / 话题 / 消息三类异常）
- INFO / WARN / ERROR 分级日志
- 日志同时输出到控制台并持久化写入 log.txt 文件
- 带温度、湿度、时间戳的传感器扩展消息 SensorMessage

## 2. 运行环境
- 操作系统：Windows
- 编译器：MinGW-w64（g++），并将 MinGW 的 bin 目录加入系统环境变量 PATH
- 标准：C++11 及以上
- 依赖：仅使用标准 C++ 库，无第三方依赖

## 3. 源码文件说明
| 文件 | 说明 |
| --- | --- |
| main.cpp | 程序入口，组织 8 个测试场景：基础通信、传感器消息、异常测试、单例验证 |
| Node.h | 抽象节点基类，定义 init()、shutdown() 生命周期纯虚接口 |
| Publisher.h / Publisher.cpp | 发布者类，绑定话题并向核心发送消息 |
| Subscriber.h / Subscriber.cpp | 订阅者类，订阅话题并接收回调处理 |
| Message.h | 基础消息类，内部自动生成时间戳，数据只读 |
| SensorMessage.h | 传感器扩展消息（温度、湿度、时间戳） |
| RosCore.h / RosCore.cpp | 全局核心调度器（单例），维护话题-订阅者/发布者映射 |
| RosException.h | 自定义异常：NodeException / TopicException / MessageException |
| Log.h | 分级日志模块（单例），控制台 + 文件双输出 |
| log.txt | 程序运行后自动生成，保存全部 INFO / WARN / ERROR 日志 |

## 4. 编译方法（Windows CMD）

### 4.1 打开命令提示符
按 `Win + R`，输入 `cmd`，回车，打开命令提示符窗口。

### 4.2 进入源码目录
```cmd
cd /d D:\ros_sim\src
```

### 4.3 编译
```cmd
g++ -std=c++11 main.cpp RosCore.cpp Publisher.cpp Subscriber.cpp -o ros_sim.exe
```
编译无报错即生成 `ros_sim.exe`。

## 5. 运行方法
```cmd
ros_sim.exe
```
程序依次执行 8 个测试场景，控制台打印全部分级日志；
同一目录下自动生成 `log.txt`，内容与控制台日志一致，可用于提交时佐证运行结果。

## 6. 输出说明
日志格式统一为：`[时间] [等级] 内容`
- `INFO`：节点注册、发布、接收、注销、场景开始结束等正常流程
- `WARN`：注销时找不到话题或节点等非致命情况
- `ERROR`：捕获到 NodeException / TopicException / MessageException 时打印异常信息

## 7. 常见问题
1. 提示 `g++ 不是内部或外部命令`：说明 MinGW 的 bin 目录未加入 PATH，需要配置环境变量或使用绝对路径。
2. 中文显示乱码：程序内部已调用 `SetConsoleOutputCP(CP_UTF8)` 设置控制台编码为 UTF-8，若仍乱码请确认终端字体支持中文。
3. 找不到 log.txt：日志写在**程序运行时的当前目录**，请在运行 `ros_sim.exe` 的目录下查找。