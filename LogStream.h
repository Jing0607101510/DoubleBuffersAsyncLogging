#ifndef __LOG_STREAM_H__
#define __LOG_STREAM_H__

#include <string>
#include <functional>

#include "Buffer.h"

const int k_stream_buffer_size =  1024 * 16;    // const全局变量的链接性是内部的
const int k_num_max_bit_count = 32;

class LogStream {
    public:
        typedef std::function<void(const char*, int)> FlushFunc;

        LogStream& operator<<(bool);
        LogStream& operator<<(char);
        LogStream& operator<<(const char*);
        LogStream& operator<<(const unsigned char*);
        LogStream& operator<<(const std::string&);
        LogStream& operator<<(short);
        LogStream& operator<<(unsigned short);
        LogStream& operator<<(int);
        LogStream& operator<<(unsigned int);
        LogStream& operator<<(long);
        LogStream& operator<<(unsigned long);
        LogStream& operator<<(long long);
        LogStream& operator<<(unsigned long long);
        LogStream& operator<<(float);
        LogStream& operator<<(double);
        LogStream& operator<<(long double);
        LogStream& operator<<(LogStream& (*__pf)(LogStream&));

        void Append(const char* content, int len);
        void ResetBuffer();
        void SetFlushFunc(FlushFunc flush_func);
        void Flush();

    private:
        typedef Buffer<k_stream_buffer_size> StreamBuffer;
        
        FlushFunc flush_func_;
        StreamBuffer buffer_;
};

#endif