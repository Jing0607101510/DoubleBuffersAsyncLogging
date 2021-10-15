#ifndef __LOG_STREAM_H__
#define __LOG_STREAM_H__

#include <string>

#include "StreamBuffer.h"

const int k_stream_buffer_size =  1024 * 16;    // const全局变量的链接性是内部的
const int k_num_max_bit_count = 32;

class LogStream {
    public:
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

        void Append(const char* content, int len);
        void ResetBuffer();

    private:
        typedef StreamBuffer<k_stream_buffer_size> Buffer;
        
        Buffer buffer_;
};

#endif