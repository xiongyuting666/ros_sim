#include"Publisher.h"
#include"Subscriber.h"
#include"Message.h"
#include"SensorMessage.h"
#include"RosCore.h"
#include"RosException.h"
#include"Log.h"
#include<iostream>
#include<string>
#include<windows.h>
using namespace std;

//统一执行一个测试场景：分别捕获三类自定义异常，
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
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    LOG_INFO("====ROS系统启动====");

    //场景1：基础通信，1个发布者 + 2个订阅者订阅同一话题，验证消息广播
    runCase("场景1 基础通信与多订阅者广播",[]()
    {
        Publisher* pub=new Publisher("pub1","/chatter");
        Subscriber* sub1=new Subscriber("sub1","/chatter");
        Subscriber* sub2=new Subscriber("sub2","/chatter");
        //启动节点：先订阅再发消息
        sub1->init();
        sub2->init();
        pub->init();
        LOG_INFO("发布者、订阅者节点初始化完成");
        //构造消息，发布
        Message msg("hello ros_sim");
        LOG_INFO("准备发布消息:hello ros_sim");
        pub->publish(msg);
        //关闭节点
        pub->shutdown();
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

    //场景3：异常测试——话题名称为空，预期抛出TopicException
    runCase("场景3 空话题名注册（预期TopicException）",[]()
    {
        Subscriber* badSub=new Subscriber("bad_sub","");
        badSub->init();
    });

    //场景4：异常测试——节点名称为空，预期抛出NodeException
    runCase("场景4 空节点名注册（预期NodeException）",[]()
    {
        Subscriber* badSub=new Subscriber("","/noname");
        badSub->init();
    });

    //场景5：异常测试——重复注册同一个订阅者，预期抛出NodeException
    runCase("场景5 重复注册同一订阅者（预期NodeException）",[]()
    {
        Subscriber* dupSub=new Subscriber("dup_sub","/dup");
        dupSub->init();
        dupSub->init();//第二次注册触发重复注册异常
    });

    //场景6：异常测试——发布空消息，预期抛出MessageException
    runCase("场景6 发布空消息（预期MessageException）",[]()
    {
        Publisher* emptyPub=new Publisher("empty_pub","/empty");
        Subscriber* emptySub=new Subscriber("empty_sub","/empty");
        emptySub->init();
        emptyPub->init();
        Message emptyMsg("");
        emptyPub->publish(emptyMsg);
    });

    //场景7：异常测试——向没有任何订阅者的话题发布消息，预期抛出TopicException
    runCase("场景7 向无订阅者话题发布（预期TopicException）",[]()
    {
        Publisher* lonelyPub=new Publisher("lonely_pub","/lonely");
        lonelyPub->init();
        lonelyPub->publish(Message("nobody listens"));
    });

    //场景8：单例验证——两次获取RosCore，验证是同一个实例
    runCase("场景8 RosCore单例验证",[]()
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

    //全局关闭：注销并关闭所有还挂着的节点
    RosCore::getInstance()->shutdownAll();
    LOG_INFO("====程序退出====");
    return 0;
}