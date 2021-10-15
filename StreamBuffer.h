#ifndef __STREAM_BUFFER_H__
#define __STREAM_BUFFER_H__

#include <string.h>

template<int buffer_size>
class StreamBuffer {
    public:
        StreamBuffer() : offset_(0) {}

        void Append(const char* content, int len) {
            int aval = Avail();
            if (len <= aval) {
                ::memcpy(buffer_ + offset_, content, len);
                offset_ += len;
            }
        }

        int Avail() {
            return buffer_size - offset_;
        }

        void Reset() {
            offset_ = 0;
        }

        char* Current() {
            return buffer_ + offset_;
        }

        void AddOffset(int n) {
            offset_ += n;
        }

        char* Data() {
            return buffer_;
        }

        int DataLen() {
            return offset_;
        }

    private:
        char buffer_[buffer_size];
        int offset_;
};

#endif