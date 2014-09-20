#include "Condition.h"
#include <errno.h>
#include <time.h>

bool MyNet::base::Condition::waitForSeconds(int seconds)
{
    struct timespec abs_time;
    clock_gettime(CLOCK_REALTIME, &abs_time);
    abs_time.tv_sec += seconds;
    MyNet::base::UnassginMutexLockGuard guard(m_mutex);
    return ETIMEDOUT == pthread_cond_timedwait(&m_cond, m_mutex.getMutex(), &abs_time); 

}
