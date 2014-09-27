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
            FixedBuffer()
                : m_cur(m_buff)
            {
                setCookie(cookieStart);
            }
            ~FixedBuffer()
            {
                setCookie(cookieEnd);
            }
            size_t append(const char* line, const size_t len);
            size_t appendPartial(const char* line, const size_t len);
            void setCookie(void (*cookie)());

            size_t avail() { return (end()- m_cur);}
            void add(size_t len) { m_cur += len;}
            void reset() { m_cur = m_buff; }
            size_t length(){ return (m_cur - m_buff);}
            char* current() { return m_cur; }
            const char* data() const { return m_buff; }
            const char* end() const { return (m_buff + sizeof(m_buff));}
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

            LogStream& operator << (const std::string& line);
            LogStream& operator << (const char* line);
            LogStream& operator << (bool b);
            LogStream& operator << (char c);
            LogStream& operator << (unsigned char uc);
            LogStream& operator << (short s);
            LogStream& operator << (unsigned short us);
            LogStream& operator << (int i);
            LogStream& operator << (unsigned int ui);
            LogStream& operator << (long l);
            LogStream& operator << (unsigned long ul);
            LogStream& operator << (long long ll);
            LogStream& operator << (unsigned long long ull);
            LogStream& operator << (float f);
            LogStream& operator << (double d);

            LogStream& operator << (const void *p);
            LogStream& operator << (const unsigned char* pc);

            size_t append(const char* line, size_t len);

        private:
            template <typename T>
            void formatInteger(T value);


            Buffer m_buff;
            static const size_t kMaxNumericSize = 32;
    };
}
}
#endif
