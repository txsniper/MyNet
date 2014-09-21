#ifndef _BASE_BOUNDED_BLOCKING_QUEUE_H_
#define _BASE_BOUNDED_BLOCKING_QUEUE_H_

#include <boost/circular_buffer.hpp>
#include "NonCopyable.h"
#include "Mutex.h"
#include "Condition.h"
#include <assert.h>

namespace MyNet
{
namespace base
{
    template <typename T>
    class BoundedBlockingQueue : private MyNet::Noncopyable
    {
        public:
            BoundedBlockingQueue(int capacity)
                : m_lock(), m_full(m_lock), m_empty(m_lock), m_queue(capacity)
            {

            }

            void put(const T& value)
            {
                MutexLockGuard(m_lock);
                while(m_queue.full())
                {
                    m_full.wait();
                }
                assert(!m_queue.full());
                m_queue.push_back(value);
                m_empty.notify();
            }

            T get()
            {
                MutexLockGuard(m_lock);
                while(m_queue.empty())
                {
                    m_empty.wait();
                }
                assert(!m_queue.empty());
                T value(m_queue.front());
                m_queue.pop_front();
                m_full.notify();
                return T;
            }

            bool empty() const
            {
                MutexLockGuard(m_lock);
                return m_queue.empty();
            }

            bool capacity() const
            {
                MutexLockGuard(m_lock);
                return m_queue.capacity();
            }

            size_t size() const
            {
                MutexLockGuard(m_lock);
                return m_queue.size();
            }
        private:
            MutexLock m_lock;
            Condition m_full;
            Condition m_empty;
            boost::circular_buffer<T> m_queue;
    };
}
}
#endif
