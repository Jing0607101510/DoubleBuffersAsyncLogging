#include <iostream>
#include <functional>
#include <unistd.h>

#include "../Logger.h"
#include "../AsyncLogging.h"

int main(void) {
    AsyncLogging async_logging(5, 1024*1024);
    async_logging.Start();

    Singleton<Logger>::GetInstance().Init(Logger::LogLevel::TRACE, 
        [&](const char* log_content, int len) {
            async_logging.Append(log_content, len);
        });

    LOG_INFO << "Hello World" << Logger::Endl;
    LOG_ERROR << "Something Error Happens" << Logger::Endl;
    LOG_FATAL << "This Program is Good!" << Logger::Endl;

    sleep(1);
    async_logging.Stop();
    sleep(1);
    async_logging.Start();

    Singleton<Logger>::GetInstance().SetLogLevel(Logger::LogLevel::INFO);
    LOG_TRACE << "Test LOG TRACE" << Logger::Endl;
    LOG_WARN << "FBI Warning!" << Logger::Endl;
    LOG_DEBUG << "Good" << Logger::Endl;
    sleep(1);
}