#ifndef _BASE_COUNT_DOWN_LATCH_H_
#define _BASE_COUNT_DOWN_LATCH_H_

#include "Mutex.h"
#include "Condition.h"
#include "NonCopyable.h"
namespace MyNet
{
namespace base
{
    class CountDownLatch: private Noncopyable
    {
        public:
            CountDownLatch(int count)
                : m_count(count), m_mutex(), m_cond(m_mutex)
            {

            }
            ~CountDownLatch(){}

            void wait();
            void countDown();
            size_t getCount();
        private:
            int m_count;
            MyNet::base::MutexLock m_mutex;
            MyNet::base::Condition m_cond;
    };
}
}
#endif
