#include"Publisher.h"
#include"Subscriber.h"
#include"Message.h"
#include"SensorMessage.h"
#include"Service.h"
#include"RosCore.h"
#include"RosException.h"
#include"Log.h"
#include<iostream>
#include<string>
#include<windows.h>
using namespace std;

//服务端业务处理函数：把请求内容加工后原样带回
ServiceResponse echoHandler(const ServiceRequest& req)
{
    ServiceResponse res("服务端已处理请求 -> "+req.content,true);
    return res;
}

//统一执行一个测试场景：分别捕获四类自定义异常，
//保证任何一个场景出错都不会让程序崩溃，后面的场景可以继续跑
template <typename CaseFunc>
void runCase(const string& caseName, CaseFunc func)
{
    LOG_INFO("---------- "+caseName+" 开始 ----------");
    try
    {
        func();
        LOG_INFO(caseName+" 执行完成");
    }
    catch(NodeException& e)
    {
        LOG_ERROR(string("节点异常：")+e.what());
    }
    catch(TopicException& e)
    {
        LOG_ERROR(string("话题异常：")+e.what());
    }
    catch(MessageException& e)
    {
        LOG_ERROR(string("消息异常：")+e.what());
    }
    catch(ServiceException& e)
    {
        LOG_ERROR(string("服务异常：")+e.what());
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    LOG_INFO("====ROS系统启动====");

    //场景1：基础通信，2个发布者 + 2个订阅者订阅同一话题，验证多发布者、多订阅者与消息广播
    runCase("场景1 多发布者多订阅者基础通信",[]()
    {
        Publisher* pub1=new Publisher("pub1","/chatter");
        Publisher* pub2=new Publisher("pub2","/chatter");
        Subscriber* sub1=new Subscriber("sub1","/chatter");
        Subscriber* sub2=new Subscriber("sub2","/chatter");
        //启动节点：先订阅再发消息
        sub1->init();
        sub2->init();
        pub1->init();
        pub2->init();
        LOG_INFO("发布者、订阅者节点初始化完成");
        //发布者连续发布消息，不需要等待订阅者响应
        pub1->publish("hello from pub1");
        pub2->publish("hello from pub2");
        //关闭节点
        pub1->shutdown();
        pub2->shutdown();
        sub1->shutdown();
        sub2->shutdown();
    });

    //场景2：传感器扩展消息，验证自定义消息类型（温度、湿度、时间戳）
    runCase("场景2 传感器扩展消息SensorMessage",[]()
    {
        Publisher* sensorPub=new Publisher("sensor_pub","/sensor");
        Subscriber* sensorSub=new Subscriber("sensor_sub","/sensor");
        sensorSub->init();
        sensorPub->init();
        SensorMessage sMsg(26.5,60.2);
        LOG_INFO("构造传感器消息，温度:"+to_string(sMsg.getTemperature())
                 +" 湿度:"+to_string(sMsg.getHumidity())
                 +" 时间戳:"+sMsg.getTimeStamp());
        sensorPub->publish(sMsg);
        sensorPub->shutdown();
        sensorSub->shutdown();
    });

    //场景3：服务端 / 客户端同步请求-响应通信
    runCase("场景3 服务同步请求响应",[]()
    {
        Service service("echo_service",echoHandler);
        RosCore::getInstance()->regService(&service);
        ServiceClient client("echo_service");
        ServiceRequest req("查询当前温度");
        ServiceResponse res=client.call(req);//同步阻塞，拿到响应后再继续
        LOG_INFO("客户端收到响应:"+res.content);
    });

    //场景4：异常测试——调用不存在的服务，预期抛出ServiceException
    runCase("场景4 调用不存在的服务（预期ServiceException）",[]()
    {
        ServiceClient client("not_exist_service");
        client.call(ServiceRequest("随便问一句"));
    });

    //场景5：运行时动态添加、删除节点
    runCase("场景5 运行时动态添加与删除节点",[]()
    {
        Publisher* dynPub=new Publisher("dyn_pub","/dynamic");
        dynPub->init();
        //运行时动态添加一个订阅者
        Subscriber* dynSub=new Subscriber("dyn_sub","/dynamic");
        dynSub->init();
        dynPub->publish("动态添加订阅者之后发布的消息");
        //运行时动态删除该订阅者
        dynSub->shutdown();
        //此时该话题下已无订阅者，再发布会抛出TopicException
        dynPub->publish("删除订阅者之后发布的消息");
        dynPub->shutdown();
    });

    //场景6：异常测试——话题名称为空，预期抛出TopicException
    runCase("场景6 空话题名注册（预期TopicException）",[]()
    {
        Subscriber* badSub=new Subscriber("bad_sub","");
        badSub->init();
    });

    //场景7：异常测试——节点名称为空，预期抛出NodeException
    runCase("场景7 空节点名注册（预期NodeException）",[]()
    {
        Subscriber* badSub=new Subscriber("","/noname");
        badSub->init();
    });

    //场景8：异常测试——重复注册同一个订阅者，预期抛出NodeException
    runCase("场景8 重复注册同一订阅者（预期NodeException）",[]()
    {
        Subscriber* dupSub=new Subscriber("dup_sub","/dup");
        dupSub->init();
        dupSub->init();//第二次注册触发重复注册异常
    });

    //场景9：异常测试——发布空消息，预期抛出MessageException
    runCase("场景9 发布空消息（预期MessageException）",[]()
    {
        Publisher* emptyPub=new Publisher("empty_pub","/empty");
        Subscriber* emptySub=new Subscriber("empty_sub","/empty");
        emptySub->init();
        emptyPub->init();
        Message emptyMsg("");
        emptyPub->publish(emptyMsg);
    });

    //场景10：异常测试——向没有任何订阅者的话题发布消息，预期抛出TopicException
    runCase("场景10 向无订阅者话题发布（预期TopicException）",[]()
    {
        Publisher* lonelyPub=new Publisher("lonely_pub","/lonely");
        lonelyPub->init();
        lonelyPub->publish(Message("nobody listens"));
    });

    //场景11：单例验证——两次获取RosCore，验证是同一个实例
    runCase("场景11 RosCore单例验证",[]()
    {
        RosCore* a=RosCore::getInstance();
        RosCore* b=RosCore::getInstance();
        if(a==b)
        {
            LOG_INFO("单例验证通过：两次获取到同一个RosCore实例，地址相同");
        }
        else
        {
            LOG_ERROR("单例验证失败：出现了多个RosCore实例");
        }
    });

    //全局关闭：遍历所有还挂着的节点，执行各自的关闭方法
    RosCore::getInstance()->shutdownAll();
    LOG_INFO("====程序退出====");
    return 0;
}