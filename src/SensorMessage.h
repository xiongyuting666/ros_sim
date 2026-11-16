#ifndef SENSORMESSAGE_H
#define SENSORMESSAGE_H
#include "Message.h"
#include <sstream>
#include <iomanip>

// 传感器扩展消息：继承基础 Message，额外携带温度、湿度，
// 时间戳仍由父类 Message 在构造时自动生成
class SensorMessage : public Message
{
public:
    SensorMessage(double temp, double hum)
        : Message(buildSensorText(temp, hum)), temperature(temp), humidity(hum)
    {
    }
    double getTemperature() const
    {
        return temperature;
    }
    double getHumidity() const
    {
        return humidity;
    }
private:
    double temperature; // 温度，单位摄氏度
    double humidity;    // 湿度，单位百分比
    // 把温度、湿度拼成可传输的消息内容，保证经由 Message 传参时不丢失
    static std::string buildSensorText(double temp, double hum)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2)
            << "[SensorMessage] temperature:" << temp
            << "C humidity:" << hum << "%";
        return oss.str();
    }
};
#endif