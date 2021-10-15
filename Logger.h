#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <functional>

#include "Singleton.h"
#include "LogStream.h"

#define LOG_TRACE if (Singleton<Logger>::GetInstance().GetLogLevel() <= Logger::LogLevel::TRACE) \
    Singleton<Logger>::GetInstance().GetLogStream(__FILE__, __LINE__, __func__, Logger::LogLevel::TRACE)
#define LOG_DEBUG if (Singleton<Logger>::GetInstance().GetLogLevel() <= Logger::LogLevel::DEBUG) \
    Singleton<Logger>::GetInstance().GetLogStream(__FILE__, __LINE__, __func__, Logger::LogLevel::DEBUG)
#define LOG_INFO  if (Singleton<Logger>::GetInstance().GetLogLevel() <= Logger::LogLevel::INFO) \
    Singleton<Logger>::GetInstance().GetLogStream(__FILE__, __LINE__, __func__, Logger::LogLevel::INFO)
#define LOG_WARN  if (Singleton<Logger>::GetInstance().GetLogLevel() <= Logger::LogLevel::WARN) \
    Singleton<Logger>::GetInstance().GetLogStream(__FILE__, __LINE__, __func__, Logger::LogLevel::WARN)
#define LOG_ERROR if (Singleton<Logger>::GetInstance().GetLogLevel() <= Logger::LogLevel::ERROR) \
    Singleton<Logger>::GetInstance().GetLogStream(__FILE__, __LINE__, __func__, Logger::LogLevel::ERROR)
#define LOG_FATAL if (Singleton<Logger>::GetInstance().GetLogLevel() <= Logger::LogLevel::FATAL) \
    Singleton<Logger>::GetInstance().GetLogStream(__FILE__, __LINE__, __func__, Logger::LogLevel::FATAL)


static const char* LogLevelName[6] = 
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};

 
class Logger {
    public:
        enum LogLevel {
            TRACE, 
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };

        typedef std::function<void(const char*, int)> OutputFunc;

    public:
        Logger(Logger::LogLevel log_level=Logger::LogLevel::INFO, Logger::OutputFunc output_func=nullptr);
        void Init(Logger::LogLevel log_level=Logger::LogLevel::INFO, Logger::OutputFunc output_func=nullptr);
        void SetLogLevel(LogLevel log_level);
        Logger::LogLevel GetLogLevel();
        
        void SetOutputFunc(OutputFunc output_func);
        LogStream& GetLogStream(const char* file, int line, const char* func, Logger::LogLevel log_level);

        static LogStream& Endl(LogStream& log_stream);

    private:
        LogLevel min_log_level_;
        OutputFunc output_func_; // 输出到什么地方，默认是终端
        LogStream log_stream_;  // 自定义的输出流

    private:
        static void DefaultOutput(const char* log_content, int len);
        static void TimeFormat(char* str_time, int len);
};

#endif