#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <mutex>

#include "Noncopyable.h"

template<class T>
class Singleton : public Noncopyable {
    public:
        static T& GetInstance() {
            if (instance_ == nullptr) {
                std::lock_guard<std::mutex> lock(mtx_);
                if (instance_ == nullptr) {
                    instance_ = new T();
                    ::atexit(&Singleton::Destroy);
                }
            }
            return *instance_;
        }
    
    private:
        Singleton() {}
        ~Singleton() {}
        
        static void Destroy() {
            if (instance_) {
                delete instance_;
                instance_ = nullptr;
            }
        }

    private:
        static T* instance_;
        static std::mutex mtx_;
};

template<class T>
T* Singleton<T>::instance_ = nullptr;

template<class T>
std::mutex Singleton<T>::mtx_;

#endif