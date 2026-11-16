# ros_sim 简易ROS发布订阅模拟器

C++ 大作业：模拟 ROS 核心通信机制，实现发布/订阅、服务通信、动态节点管理、异常体系与分级持久化日志。

## 目录结构
```
ros_sim/
├─ docs/                       提交文档
│  ├─ 需求文档.md              功能与非功能需求、验收标准
│  ├─ 架构设计文档.md          模块设计、类结构、UML、分层说明
│  ├─ 技术文档.md              开发者视角的技术选型、关键实现、已知问题
│  ├─ 用户手册.md              环境、编译、运行、常见问题
│  └─ 测试用例.md              13 条结构化用例，含实际结果
├─ src/                        源码
│  ├─ main.cpp                 程序入口，11 个测试场景
│  ├─ Node.h                   抽象节点基类
│  ├─ Publisher.h/.cpp         发布者
│  ├─ Subscriber.h/.cpp        订阅者
│  ├─ Message.h                基础消息（自动时间戳、只读）
│  ├─ SensorMessage.h          传感器扩展消息
│  ├─ Service.h                服务端/客户端
│  ├─ RosCore.h/.cpp           核心调度（单例）
│  ├─ RosException.h           四类自定义异常
│  └─ Log.h                    分级日志（单例）
├─ README.md
└─ .gitignore
```

## 环境要求
- Windows
- MinGW-w64 g++，C++11 及以上
- 无第三方依赖

## 编译
```bash
cd src
g++ -std=c++11 main.cpp RosCore.cpp Publisher.cpp Subscriber.cpp -o ros_sim.exe
```

## 运行
```bash
./ros_sim.exe
```
运行后当前目录生成 `log.txt`，记录全部 INFO / WARN / ERROR 日志。

## 功能清单
**基础功能**
1. `Node` 抽象基类：节点名 + 名称只读接口 + 纯虚 `init()` / `shutdown()`
2. `Publisher`：话题名 + 只读接口 + 注册/注销 + 发布（支持"传感器字符串 → Message → 转发"）
3. `Subscriber`：话题名 + 只读接口 + 注册/注销 + 回调，日志含消息内容与时间戳
4. `Message`：字符串内容 + 自动 `YYYY-MM-DD HH:MM:SS` 时间戳，只提供 const 读取接口
5. `RosCore`：单例，维护话题-发布者 / 话题-订阅者两张映射，统一转发与全局关闭

**系统功能**
6. 支持多发布者、多订阅者，一条消息广播给该话题全部订阅者
7. 发布接口无返回值、不阻塞等待订阅者响应
8. 节点按"初始化 → 交互 → 关闭"全生命周期运行
9. 空节点名 / 重复注册 / 空消息 / 无订阅者转发等异常均抛出自定义异常，程序不崩溃

**加分功能（全部实现）**
10. 自定义消息类型 `SensorMessage`（温度、湿度、时间戳）
11. 服务端 / 客户端同步请求-响应通信（`Service` / `ServiceClient` / `RosCore::callService`）
12. 运行时动态添加、删除节点
13. 消息收发日志持久化写入 `log.txt`
14. 四类自定义异常：`NodeException` / `TopicException` / `MessageException` / `ServiceException`
15. 全局异常捕获：`runCase()` 统一捕获，单场景失败不影响后续场景
16. 分级日志 INFO / WARN / ERROR，控制台 + 文件双输出，格式统一为 `[时间] [等级] 内容`

## 测试
程序内置 11 个场景，运行一次即可完成全部功能与异常路径验证。
详细用例见 [docs/测试用例.md](docs/测试用例.md)。

## 文档
| 文档 | 路径 |
| --- | --- |
| 需求文档 | [docs/需求文档.md](docs/需求文档.md) |
| 架构设计文档 | [docs/架构设计文档.md](docs/架构设计文档.md) |
| 技术文档 | [docs/技术文档.md](docs/技术文档.md) |
| 用户手册 | [docs/用户手册.md](docs/用户手册.md) |
| 测试用例 | [docs/测试用例.md](docs/测试用例.md) |

## 运行示例（节选）
```
[Mon Nov 16 20:43:58 2026] [INFO] ====ROS系统启动====
[Mon Nov 16 20:43:58 2026] [INFO] Subscriber register success,topic:/chatter
[Mon Nov 16 20:43:58 2026] [INFO] [Receive]subscriber:sub1 topic:/chatter content:hello from pub1 time:2026-11-16 20:43:58
[Mon Nov 16 20:43:58 2026] [INFO] [Receive]subscriber:sub2 topic:/chatter content:hello from pub1 time:2026-11-16 20:43:58
[Mon Nov 16 20:43:58 2026] [INFO] Service[echo_service]收到请求:查询当前温度
[Mon Nov 16 20:43:58 2026] [INFO] 客户端收到响应:服务端已处理请求 -> 查询当前温度
[Mon Nov 16 20:43:58 2026] [ERROR] 服务异常：ERROR:调用了不存在的服务:not_exist_service
[Mon Nov 16 20:43:58 2026] [INFO] ====程序退出====
```