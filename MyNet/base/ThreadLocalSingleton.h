#ifndef _BASE_THREAD_LOCAL_SINGLETON_H_
#define _BASE_THREAD_LOCAL_SINGLETON_H_

#include "NonCopyable.h"
#include <pthread.h>
namespace MyNet
{

namespace base
{
    template <typename T>
    class ThreadLocalSingleton : private MyNet::Noncopyable
    {
        public:
            static T& instance()
            {
                if(!m_t_value)
                {
                    m_t_value = new T();
                    m_deleter.set(m_t_value);
                }
                return *m_t_value;
            }

            static T* pointer()
            {
                return m_t_value;
            }

        private:
            ThreadLocalSingleton();
            ~ThreadLocalSingleton();

            static void destructor(void *obj)
            {
                assert(obj == m_t_value);
                T* t_obj = static_cast<T*>(obj);
                typedef char TMustBeCompleteType[sizeof(T) ? -1:];
                TMustBeCompleteType dummy;
                (void)dummy;
                delete t_obj;
            }
            class Deleter
            {
                public:
                    Deleter()
                    {
                        ::pthread_key_create(&m_pkey, &ThreadLocalSingleton::destructor);
                    }

                    ~Deleter()
                    {
                        ::pthread_key_delete(&m_pkey);
                    }

                    void set(T* obj)
                    {
                        assert(::pthread_getspecific(m_pkey) == NULL);
                        ::pthread_setspecific(m_pkey, obj);
                    }

                private:
                    ::pthread_key_t m_pkey;
            };
            static __thread T* m_t_value;
            static Deleter m_deleter;
    };

    template<typename T>
    __thread T* ThreadLocalSingleton<T>::m_t_value = 0;

    template<typename T>
    ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::m_deleter;
}
}
#endif
