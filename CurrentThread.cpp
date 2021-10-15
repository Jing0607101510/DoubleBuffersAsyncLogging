#include <sys/prctl.h>
#include <thread>
#include <unistd.h>
#include <sys/syscall.h>

#include "CurrentThread.h"

namespace CurrentThread {
    __thread int t_cachedTid = 0;

    void CacheTid() {
        t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
    }
}