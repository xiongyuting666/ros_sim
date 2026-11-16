# ros_sim 简易ROS发布订阅模拟器

## 项目说明
模拟ROS发布订阅通信模型，包含 Node、Publisher、Subscriber、Message、SensorMessage、RosCore 六大部分。
RosCore 作为全局调度中心，维护话题与发布者/订阅者的映射关系，负责消息转发。
程序运行后在控制台打印分级日志，同时把全部日志持久化写入 `log.txt`。

## 环境要求
- Windows
- MinGW-w64 g++，支持 C++11 及以上
- 无第三方依赖

## 文件结构
```
src/
|-----main.cpp              程序入口，8个测试场景
|-----Node.h                抽象节点基类，定义 init()/shutdown() 纯虚接口
|-----Message.h             基础消息类，带自动生成的时间戳，数据只读
|-----SensorMessage.h       传感器扩展消息（温度、湿度、时间戳）
|-----Publisher.h/.cpp      发布者，绑定话题并发布消息
|-----Subscriber.h/.cpp     订阅者，订阅话题并回调处理消息
|-----RosCore.h/.cpp        核心调度器（单例），注册/注销/消息转发
|-----RosException.h        三类自定义异常：NodeException / TopicException / MessageException
|-----Log.h                 分级日志单例，INFO/WARN/ERROR，控制台 + log.txt 双输出
```

## 编译命令
进入 `src` 目录，终端执行：
```bash
g++ -std=c++11 main.cpp RosCore.cpp Publisher.cpp Subscriber.cpp -o ros_sim.exe
```

## 运行
```bash
./ros_sim.exe
```
运行后当前目录会生成 `log.txt`，记录全部 INFO / WARN / ERROR 日志。

## 功能说明
1. 基础发布订阅：发布者绑定话题，订阅者订阅同一话题，消息经 RosCore 转发后回调到订阅者
2. 多订阅者广播：同一话题注册多个订阅者，一条消息全部收到
3. 消息自动携带时间戳，接收端日志同时打印内容和时间
4. 传感器扩展消息 SensorMessage：携带温度、湿度、时间戳，模拟传感器上报
5. 三类自定义异常校验：空节点名、重复注册 → NodeException；空话题、向无订阅者话题发布 → TopicException；空消息 → MessageException
6. 分级日志：INFO 记录正常流程，WARN 记录注销异常情况，ERROR 记录异常，全部写入 log.txt
7. RosCore 单例：全局唯一实例，构造函数私有化
8. Node 抽象基类：纯虚 init()/shutdown()，不可直接实例化
9. Message 数据只读：私有成员只提供 const 读取接口
10. 程序结尾调用 shutdownAll() 统一注销并关闭所有节点

## 运行示例输出（节选）
```
[Mon Nov 16 20:37:34 2026] [INFO] ====ROS系统启动====
[Mon Nov 16 20:37:34 2026] [INFO] Subscriber register success,topic:/chatter
[Mon Nov 16 20:37:34 2026] [INFO] Publisher register success,topic:/chatter
[Mon Nov 16 20:37:34 2026] [INFO] [Receive]subscriber:sub1 topic:/chatter content:hello ros_sim time:2026-11-16 20:37:34
[Mon Nov 16 20:37:34 2026] [INFO] [Receive]subscriber:sub2 topic:/chatter content:hello ros_sim time:2026-11-16 20:37:34
[Mon Nov 16 20:37:34 2026] [INFO] [Publish]topic:/chatter content:hello ros_sim time:2026-11-16 20:37:34
[Mon Nov 16 20:37:34 2026] [ERROR] 话题异常：ERROR:话题名称不能为空
[Mon Nov 16 20:37:34 2026] [INFO] ====程序退出====
```