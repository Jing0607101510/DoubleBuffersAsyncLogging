#include <functional>
#include <chrono>
#include <assert.h>
#include <iostream>

#include "AsyncLogging.h"

AsyncLogging::AsyncLogging(int flush_interval, long roll_size) 
    : flush_interval_(flush_interval),
      roll_size_(roll_size),
      running_(false),
      cur_buffer_(new LogContentBuffer()),
      next_buffer_(new LogContentBuffer()),
      log_file_(flush_interval, roll_size) {
}

AsyncLogging::~AsyncLogging() {
    if (running_) {
        Stop();
    }
}

void AsyncLogging::Append(const char* log_content, int len) {
    std::unique_lock<std::mutex> lock(mtx_);
    if (cur_buffer_->Avail() >= len) {
        cur_buffer_->Append(log_content, len);
    }
    else {
        buffer_vec_.push_back(std::move(cur_buffer_));
        
        if (next_buffer_) {
            cur_buffer_ = std::move(next_buffer_);
        }
        else {
            cur_buffer_.reset(new LogContentBuffer);
        }
        cur_buffer_->Append(log_content, len);
        cond_.notify_all();
    }
}

void AsyncLogging::Start() {
    if (!running_) {
        running_ = true;
        thread_ = std::thread(std::bind(&AsyncLogging::ThreadFunc, this));
    }
}

void AsyncLogging::Stop() {
    if (running_) {
        running_ = false;
        cond_.notify_one();
        thread_.join();
    }
}

void AsyncLogging::ThreadFunc() {
    assert(running_ == true);

    BufferPtr new_buffer1(new LogContentBuffer());
    BufferPtr new_buffer2(new LogContentBuffer());

    std::vector<BufferPtr> buffers_to_write;

    while (running_) {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            if (buffer_vec_.empty()) {
                cond_.wait_for(lock, std::chrono::seconds(flush_interval_));
            }
            buffer_vec_.push_back(std::move(cur_buffer_));
            buffers_to_write.swap(buffer_vec_);

            cur_buffer_ = std::move(new_buffer1);
            if (!next_buffer_) {
                next_buffer_ = std::move(new_buffer2);
            }
        }

        if (buffers_to_write.size() > 26) {
            char msg[256];
            snprintf(msg, sizeof(msg), "Too much buffers. Dropping %d buffers.", buffers_to_write.size()-2);
            fputs(msg, stderr);
            log_file_.Append(msg, strlen(msg));
            buffers_to_write.erase(buffers_to_write.begin() + 2, buffers_to_write.end());
        }

        for (auto& buffer_ptr : buffers_to_write) {
            log_file_.Append(buffer_ptr->Data(), buffer_ptr->DataLen());
        }

        if (!new_buffer1) {
            assert(!buffers_to_write.empty());
            new_buffer1 = std::move(buffers_to_write.back());
            buffers_to_write.pop_back();
            new_buffer1->Reset();
        }

        if (!new_buffer2) {
            assert(!buffers_to_write.empty());
            new_buffer2 = std::move(buffers_to_write.back());
            buffers_to_write.pop_back();
            new_buffer2->Reset();
        }

        buffers_to_write.clear();
    }
    
    log_file_.Flush();
}

