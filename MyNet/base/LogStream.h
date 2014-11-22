#ifndef _BASE_LOG_STREAM_H_
#define _BASE_LOG_STREAM_H_

#include "NonCopyable.h"
#include <string>
#include <string.h>
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
                size_t append(const char* line, const size_t len)
                {
                    if(avail() >= len)
                    {
                        memcpy(m_cur, line, len);
                        m_cur += len;
                        return len;
                    }
                    else
                    {
                        return 0;
                    }
                }

                size_t appendPartial(const char* line, const size_t len)
                {
                    if(avail() >= len)
                    {
                        memcpy(m_cur, line, len);
                        m_cur += len;
                        return len;
                    }
                    else
                    {
                        size_t nappend = avail();
                        memcpy(m_cur, line, nappend);
                        m_cur += nappend;
                        return nappend;
                    }
                }
                void setCookie(void (*cookie)())
                {
                    m_cookie = cookie;
                }

                size_t avail() const { return (end()- m_cur);}
                void add(size_t len) { m_cur += len;}
                void reset() { m_cur = m_buff; }
                size_t length() const { return (m_cur - m_buff);}
                char* current() { return m_cur; }
                const char* data() const { return m_buff; }
                const char* end() const { return (m_buff + sizeof(m_buff));}

                std::string asString() const
                {
                    return std::string(data(), length());
                }
           private:
                static void cookieStart()
                {

                }
                static void cookieEnd()
                {

                }
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

                const Buffer& buffer() const
                {
                    return m_buff;
                }

                void resetBuffer()
                {
                    m_buff.reset();
                }
            private:
                template <typename T>
                    void formatInteger(T value);
                Buffer m_buff;
                static const size_t kMaxNumericSize = 32;
        };

        class Fmt
        {
            public:
                template<typename T>
                Fmt(const char* fmt, T val);
                const char* data() const {return m_buf;}
                int length() const { return m_len;}
            private:
                char m_buf[32];
                int m_len;
        };


        inline LogStream& operator << (LogStream& s, const Fmt& fmt)
        {
            s.append(fmt.data(), fmt.length());
            return s;
        }
    }
}
#endif
