#ifndef _BASE_MUTEX_
#define _BASE_MUTEX_
#include <pthread.h>
#include <assert.h>
#include "ThreadOp.h"
#include "NonCopyable.h"
namespace MyNet
{
namespace base
{
    class MutexLock : private Noncopyable
    {
        public:
            MutexLock()
                :m_holder(0)
            {
                pthread_mutex_init(&m_lock, NULL);
            }
            ~MutexLock()
            {
                assert(m_holder == 0);
                pthread_mutex_destroy(&m_lock);
            }
            void lock()
            {
                assert(m_holder == 0);
                m_holder = MyNet::ThreadOp::tid();
                pthread_mutex_lock(&m_lock);
            }

            void unlock()
            {
                m_holder = 0;
                pthread_mutex_unlock(&m_lock);
            }
            // only used in Condition
            void unassgin()
            {
                m_holder = 0;
            }
            // only used in Condition
            pthread_mutex_t* getMutex()
            {
                return  &m_lock;
            }
        private:
            pthread_mutex_t m_lock;
            pid_t m_holder;

    };

    class MutexLockGuard
    {
        public:
            MutexLockGuard(MutexLock& mutex)
                : m_mutex(mutex)
            {
                m_mutex.lock();
            }
            ~MutexLockGuard()
            {
                m_mutex.unlock();
            }
        private:
            MutexLock m_mutex;
    };

    class UnassginMutexLockGuard
    {
        public:
            UnassginMutexLockGuard(MutexLock& mutex)
                : m_mutex(mutex)
            {
                m_mutex.unassgin();
            }

            ~UnassginMutexLockGuard()
            {

            }
        private:
            MutexLock m_mutex;
    };
}
}
#endif
