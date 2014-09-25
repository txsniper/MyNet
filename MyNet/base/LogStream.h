#ifndef _BASE_LOG_STREAM_H_
#define _BASE_LOG_STREAM_H_

#include "NonCopyable.h"
#include <string>
namespace MyNet
{
namespace base
{
    const int kSmallBuffer = 4000;
    const int kLargeBuffer = 4000 * 1000;
    template <int SIZE>
    class FixedBuffer : private MyNet::Noncopyable
    {
        public:
            FixedBuffer();
            ~FixedBuffer();
            size_t append(const char* line, const size_t length);
            size_t appendPartial(const char* line, const size_t length);
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

    class LogStream : private MyNet::Noncopyable
    {
        public:
            typedef FixedBuffer<kSmallBuffer> Buffer;

            LogStream& operator << (const std::string line);
            LogStream& operator << (const char* line);
            LogStream& operator << (bool b);
            LogStream& operator << (int i);
            LogStream& operator << (long i);

        private:
            Buffer m_buff;
    };
}
}
#endif
