#ifndef __ASYNC_LOGGING_H__
#define __ASYNC_LOGGING_H__

#include <atomic>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "Buffer.h"
#include "LogFile.h"

const int k_buffer_size = 1024 * 16;

class AsyncLogging {
    public:
        AsyncLogging(int flush_interval=3, long roll_size=1024*1024*1024);
        ~AsyncLogging();
        void Append(const char* log_content, int len);
        void Start();
        void Stop();

    private:
        void ThreadFunc();

    private:
        typedef Buffer<k_buffer_size> LogContentBuffer;
        typedef std::unique_ptr<LogContentBuffer> BufferPtr;

        const int flush_interval_;  // 前后端buffer交换的时间
        const long roll_size_;

        std::atomic<bool> running_;
        
        std::thread thread_;

        std::mutex mtx_;
        std::condition_variable cond_;

        BufferPtr cur_buffer_;
        BufferPtr next_buffer_;
        std::vector<BufferPtr> buffer_vec_;

        LogFile log_file_;
}; 

#endif