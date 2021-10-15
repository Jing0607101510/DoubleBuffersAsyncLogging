#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <thread>

#include "Logger.h"

Logger::Logger(Logger::LogLevel log_level, Logger::OutputFunc output_func) {
    min_log_level_ = log_level;
    SetOutputFunc(output_func);
}

void Logger::SetLogLevel(Logger::LogLevel log_level) {
    min_log_level_ = log_level;
}

Logger::LogLevel Logger::GetLogLevel() {
    return min_log_level_;
}

void Logger::SetOutputFunc(Logger::OutputFunc output_func) {
    if (output_func) {
        output_func_ = output_func;
    }
    else {
        output_func = Logger::DefaultOutput;
    }
}

LogStream& Logger::GetLogStream(const char* file, int line, const char* func, Logger::LogLevel log_level) {
    if (log_level >= min_log_level_) {
        char str_time[26] = {0};
        log_stream_ << str_time;
        log_stream_ << file << ' ' << line << ' ' << func << ' ';
        log_stream_ << CurrentThread::tidString() << ' ';
        log_stream_ << LogLevelName[log_level];
    }
    return log_stream_;
}

void Logger::DefaultOutput(const char* log_content, int len) {
    fwrite(log_content, 1, len, stdout);
}

void Logger::TimeFormat(char* str_time, int len) {
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    time_t time;;
    time = tv.tv_sec;
    struct tm* p_time = localtime(&time);

    strftime(str_time, 26, "%Y-%m-%d %H:%M:%S ", p_time);
}