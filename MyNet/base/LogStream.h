#ifndef _BASE_LOG_STREAM_H_
#define _BASE_LOG_STREAM_H_

#include "NonCopyable.h"
#include <string>
namespace MyNet
{
namespace base
{
    template <int SIZE>
    class FixedBuffer : private MyNet::Noncopyable
    {
        public:
            FixedBuffer();
            ~FixedBuffer();
            size_t append(const char* line, const size_t length);
            void setCookie(void (*cookie)());

            size_t avail() { return (end()- m_cur);}
            void reset() { m_cur = m_buff; }
            size_t length(){ return (m_cur - m_buff);}
            const char* current() { return m_cur; }
            const char* data() const { return m_buff; }
            size_t end() const { return (m_buff + sizeof(m_buff));}
        private:
            void cookieStart();
            void cookieEnd();
        private:
            void (*m_cookie)();
            char* m_cur;
            char m_buff[SIZE];

    };
}
}
#endif
