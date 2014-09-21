#ifndef _BASE_CONDITION_H_
#define _BASE_CONDITION_H_
#include "Mutex.h"
#include "NonCopyable.h"
namespace MyNet
{
namespace base
{
    class Condition:private MyNet::Noncopyable
    {
        public:
            Condition(MutexLock& mutex)
               : m_mutex(mutex)
            {
                pthread_cond_init(&m_cond, NULL);
            }

            ~Condition()
            {
                pthread_cond_destroy(&m_cond);
            }

            void wait()
            {
                pthread_cond_wait(&m_cond, m_mutex.getMutex());
            }

            void notify()
            {
                pthread_cond_signal(&m_cond);
            }

            void notifyAll()
            {
                pthread_cond_broadcast(&m_cond);
            }

            bool waitForSeconds(int seconds);
        private:
            pthread_cond_t m_cond;
            MutexLock& m_mutex;
    };
}
}
#endif
