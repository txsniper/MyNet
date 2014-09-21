#ifndef _BASE_ATOMIC_
#define _BASE_ATOMIC_

#include "NonCopyable.h"
#include <stdint.h>
namespace MyNet
{
namespace base
{
    template <typename T>
    class AtomicIntegerT: private MyNet::Noncopyable
    {
        public:
            AtomicIntegerT(T value)
                : m_value(value)
            {

            }
            AtomicIntegerT()
                : m_value(0)
            {

            }
            ~AtomicIntegerT(){}

            T get()
            {
                return __sync_val_compare_and_swap(&m_value, 0, 0);
            }

            T getAndSet(T value)
            {
                return __sync_lock_test_and_set(&m_value, value);
            }

            T addAndGet(T value)
            {
                return __sync_add_and_fetch(&m_value, value);
            }

            T getAndAdd(T value)
            {
                return addAndGet(value) - value;
            }

            T incAndGet()
            {
                return addAndGet(1);
            }

            void inc()
            {
                addAndGet(1);
            }

            T subAndGet(T value)
            {
               return __sync_sub_and_fetch(&m_value, value);
            }

            T decAndGet()
            {
                return subAndGet(1);
            }

            void dec()
            {
                subAndGet(1);
            }
        private:
            volatile T m_value;

    };
    typedef AtomicIntegerT<int32_t> AtomicInt32;
    typedef AtomicIntegerT<int64_t> AtomicInt64;
}
}
#endif
