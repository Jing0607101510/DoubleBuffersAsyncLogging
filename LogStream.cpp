#include <cstring>

#include "LogStream.h"

LogStream& LogStream::operator<<(bool val) {
    const char* str_val = (val ? "true" : "false");
    buffer_.Append(str_val, ::strlen(str_val));
    return *this;
}

LogStream& LogStream::operator<<(char val) {
    buffer_.Append(&val, 1);
    return *this;
}

LogStream& LogStream::operator<<(const char* str) {
    if (str) {
        buffer_.Append(str, ::strlen(str));
    } 
    else {
        const char* str_val = "(null)";
        buffer_.Append(str_val, ::strlen(str_val));
    }
    return *this;
}

LogStream& LogStream::operator<<(const unsigned char* ustr) {
    const char* str = reinterpret_cast<const char*>(ustr);
    *this << str;
    return *this;
}

LogStream& LogStream::operator<<(const std::string& str) {
    buffer_.Append(str.c_str(), str.size());
    return *this;
}

LogStream& LogStream::operator<<(short val) {
    int write_len = snprintf(buffer_.Current(), buffer_.Avail(), "%hd", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short val) {
    int write_len = snprintf(buffer_.Current(), buffer_.Avail(), "%hu", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(int val) {
    int write_len = snprintf(buffer_.Current(), buffer_.Avail(), "%d", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int val) {
    int write_len = snprintf(buffer_.Current(), buffer_.Avail(), "%u", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(long val) {
    int write_len = snprintf(buffer_.Current(), buffer_.Avail(), "%ld", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long val) {
    int write_len = snprintf(buffer_.Current(), buffer_.Avail(), "%lu", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(long long val) {
    int write_len = snprintf(buffer_.Current(), buffer_.Avail(), "%lld", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long val) {
    int write_len = snprintf(buffer_.Current(), buffer_.Avail(), "%llu", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(float val) {
    double dval = static_cast<double>(val);
    *this << dval;
    return *this;
}

LogStream& LogStream::operator<<(double val) {
    int write_len = ::snprintf(buffer_.Current(), buffer_.Avail(), "%.12g", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(long double val) {
    int write_len = ::snprintf(buffer_.Current(), buffer_.Avail(), "%.12Lg", val);
    buffer_.AddOffset(write_len);
    return *this;
}

LogStream& LogStream::operator<<(LogStream& (*__pf)(LogStream&)) {
    return __pf(*this);
}


void LogStream::Append(const char* content, int len) {
    buffer_.Append(content, len);
}

void LogStream::ResetBuffer() {
    buffer_.Reset();
}

void LogStream::Flush() {
    if (flush_func_) {
        flush_func_(buffer_.Data(), buffer_.DataLen());
    }
    ResetBuffer();
}

void LogStream::SetFlushFunc(LogStream::FlushFunc flush_func) {
    flush_func_ = flush_func;
}