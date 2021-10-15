#ifndef __CURRENT_THREAD_H__
#define __CURRENT_THREAD_H__

namespace CurrentThread {
    extern __thread int t_cachedTid;

    void CacheTid();

    inline int tid() {
        if (__builtin_expect(t_cachedTid == 0, 0)) {
            CacheTid();
        }
        return t_cachedTid;
    }
    
}

#endif