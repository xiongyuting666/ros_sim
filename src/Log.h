#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
enum class LogLevel
{
    INFO,
    WARN,
    ERR
};
class Log
{
public:
    static Log& getInstance()
    {
        static Log log;
        return log;
    }
    void log(LogLevel level, const std::string& msg)
    {
        std::string levelStr;
        switch (level)
        {
            case LogLevel::INFO:
                levelStr = "INFO";
                break;
            case LogLevel::WARN:
                levelStr = "WARN";
                break;
            case LogLevel::ERR:
                levelStr = "ERROR";
                break;
        }
        time_t now = time(nullptr);
        std::string timeStr = std::ctime(&now);
        if (!timeStr.empty() && timeStr.back() == '\n')
        {
            timeStr.pop_back(); // 去掉 ctime 末尾的换行符
        }
        // 统一日志格式：[时间] [等级] 内容
        std::string out = "[" + timeStr + "] [" + levelStr + "] " + msg;
        std::cout << out << std::endl;
        if (file.is_open())
        {
            file << out << std::endl;
        }
    }
private:
    std::ofstream file;
    Log()
    {
        // 日志文件生成在程序运行目录下
        file.open("log.txt", std::ios::app);
    }
    ~Log()
    {
        if (file.is_open())
        {
            file.close();
        }
    }
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;
};
#define LOG_INFO(msg) Log::getInstance().log(LogLevel::INFO, msg)
#define LOG_WARN(msg) Log::getInstance().log(LogLevel::WARN, msg)
#define LOG_ERROR(msg) Log::getInstance().log(LogLevel::ERR, msg)
#endif