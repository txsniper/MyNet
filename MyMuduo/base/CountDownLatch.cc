#include "CountDownLatch.h"

void MyNet::base::CountDownLatch::wait()
{
    MutexLockGuard(m_mutex);
    while(m_count > 0)
        m_cond.wait();
}

void MyNet::base::CountDownLatch::countDown()
{
    MutexLockGuard(m_mutex);
    --m_count;
    if(m_count == 0)
        m_cond.notifyAll();
}

size_t MyNet::base::CountDownLatch::getCount()
{
    MutexLockGuard(m_mutex);
    return m_count;
}
