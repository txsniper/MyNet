#ifndef _BASE_THREAD_LOCAL_H_
#define _BAEE_THREAD_LOCAL_H_

#include "NonCopyable.h"
#include <pthread.h>
namespace MyNet
{
namespace base
{
    template <typename T>
    class ThreadLocal : private MyNet::Noncopyable
    {
        ThreadLocal()
        {
            ::pthread_key_create(&m_pkey, &ThreadLocal::destructor);
        }
        ~ThreadLocal()
        {
            ::pthread_key_delete(&m_pkey);
        }

        T& value()
        {
            T* value = static_cast<T*>(::pthread_getspecific(m_pkey));
            if(!value)
            {
                T* new_value = new T();
                ::pthread_setspecific(m_pkey, new_value);
                value = new_value;
            }
            return *value;
        }

        private:
        static void destructor(void *obj)
        {
            T* t_obj = static_cast<T*>(obj);
            typedef char TMustBeCompleteType[sizeof(T) ? -1 : 1];
            TMustBeCompleteType dummy;
            (void)dummy;
            delete t_obj;
        }

        private:
            pthread_key_t m_pkey;
    };

}
}
#endif
