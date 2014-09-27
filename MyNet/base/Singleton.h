#ifndef _BASE_SINGLETON_H_
#define _BASE_SINGLETON_H_

#include <pthread.h>
#include <stdlib.h>
#include "NonCopyable.h"
namespace MyNet
{
namespace base
{
    template <typename T>
    class Singleton : private MyNet::Noncopyable
    {
        public:
            static T& instance()
            {
                pthread_once(&m_ponce, &Singleton::Init);
                return *m_value;
            }
        private:
            Singleton();
            ~Singleton();
            static void Init()
            {
                m_value = new T();
                ::atexit(Destroy);
            }

            static void Destroy()
            {
                typedef char T_must_be_complete_type[sizeof(T) ? -1 : 1];
                T_must_be_complete_type dummy;
                (void)dummy;
                if(m_value)
                {
                    delete m_value;
                }
            }
        private:
            static pthread_once_t  m_ponce;
            static T* m_value;
    };

    template <typename T>
    pthread_once_t Singleton<T>::m_ponce = PTHREAD_ONCE_INIT;

    template <typename T>
    T* Singleton<T>::m_value = NULL;
}
}
#endif
