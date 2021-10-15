#ifndef __LOG_FILE_H__
#define __LOG_FILE_H__

#include <stdio.h>
#include <string>

class LogFile {
    public:
        LogFile(int flush_interval=3, long roll_size=1024*1024*1024);
        void Append(const char* content, int len);
        void Flush();
        void RollFile();

    private:
        std::string GetNewLogFileName(time_t* now);
        void WriteToFile(const char* content, int len);
        const int flush_interval_;
        const long roll_size_;
        const std::string pid_;

        long byte_written_; // 已经写入当前日志文件的字节数
        long cur_period_;   // 天数
        long last_flush_;
        int file_id_;    // 同一天由于日志量过大而划分出多个日志文件，导致需要id区分
        FILE* file_;
};

#endif