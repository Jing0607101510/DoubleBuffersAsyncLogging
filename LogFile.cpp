#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

#include "LogFile.h"

__thread char t_errnobuf[512];

LogFile::LogFile(int flush_interval, long roll_size)
    : flush_interval_(flush_interval),
      roll_size_(roll_size),
      pid_(std::to_string(::getpid())),
      file_(nullptr),
      byte_written_(0), // 当前文件已经写入多少字节
      cur_period_(0),   // 天数
      last_flush_(0),   // 上一次刷新
      file_id_(0) {     // 当天的第几个
    RollFile();
}

void LogFile::Append(const char* content, int len) {
    WriteToFile(content, len);
    // 大小是否超出限制
    if (byte_written_ >= roll_size_) {
        RollFile();
    }
    else {
        time_t now;
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        now = tv.tv_sec;
        long new_period = now / (60 * 60 * 24);
        // 是否到达第二天了
        if (new_period > cur_period_) {
            RollFile();
        }
        else if (now - last_flush_ >= flush_interval_) {
            last_flush_ = now;
            fflush(file_);
        }
    }
}

void LogFile::WriteToFile(const char* content, int len) {
    int written = 0;
    int cnt = 0;
    while (written < len) {
        cnt = fwrite(content + written, 1, len - written, file_);
        if (cnt == 0) {
            int err = ferror(file_);
            if (err) {
                fprintf(stderr, "LogFile::Append() failed %s \n", strerror_r(err, t_errnobuf, sizeof t_errnobuf));
            }
            break;
        }
        written += cnt;
    }
    byte_written_ += written;
}

void LogFile::Flush() {
    fflush(file_);
}

void LogFile::RollFile() {
    time_t now = 0;
    std::string log_file_name = GetNewLogFileName(&now);
    long new_period = now / (60 * 60 * 24);
    
    if(file_) {
        Flush();
        fclose(file_);     
        file_= nullptr;   
    }
    
    byte_written_ = 0;
    last_flush_ = now;
    if (new_period > cur_period_) {
        file_id_ = 0;
        cur_period_ = new_period;
    }
    file_id_++;

    log_file_name += std::to_string(file_id_) + ".log";
    file_ = fopen(log_file_name.c_str(), "a+");
}

std::string LogFile::GetNewLogFileName(time_t* now) {
    std::string file_name;

    struct timeval tv;
    gettimeofday(&tv, nullptr);

    *now = tv.tv_sec;
    struct tm* p_time = localtime(now);

    char str_time[26];
    strftime(str_time, 26, "%Y-%m-%d", p_time);

    file_name = pid_ + " " + str_time + " ";

    return file_name;
}