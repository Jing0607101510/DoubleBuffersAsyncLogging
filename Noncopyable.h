#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

class Noncopyable {
    public:
        Noncopyable(Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
    
    protected:
        Noncopyable() = default;
        ~Noncopyable() = default;
};  

#endif
