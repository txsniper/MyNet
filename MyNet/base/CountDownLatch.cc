#include "CountDownLatch.h"

void MyNet::base::CountDownLatch::wait()
{
    MutexLockGuard guard(m_mutex);
    while(m_count > 0)
        m_cond.wait();
}

void MyNet::base::CountDownLatch::countDown()
{
    MutexLockGuard guard(m_mutex);
    --m_count;
    if(m_count == 0)
        m_cond.notifyAll();
}

size_t MyNet::base::CountDownLatch::getCount()
{
    MutexLockGuard guard(m_mutex);
    return m_count;
}
