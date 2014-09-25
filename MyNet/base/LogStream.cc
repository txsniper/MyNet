#include "LogStream.h"
namespace MyNet
{
namespace base
{
    FixedBuffer::FixedBuffer()
        : m_cur(m_buff)
    {
        setCookie(cookieStart);
    }

    FixedBuffer::~FixedBuffer()
    {
        setCookie(cookieEnd);
    }

    void FixedBuffer::setCookie(void (*cookie)())
    {
        m_cookie = cookie;
    }

    size_t FixedBuffer::append(const char* line, const size_t length)
    {
        if(avail() >= length)
        {
            memcpy(m_cur, line, length);
            m_cur += length;
            return length;
        }
        else
        {
            return 0;
        }
    }

    size_t FixedBuffer::appendPartial(const char* line, const size_t length)
    {
        if(avail() >= length)
        {
            memcpy(m_cur, line, length);
            m_cur += length;
            return length;
        }
        else
        {
            size_t nappend = avail();
            memcpy(m_cur, line, nappend);
            m_cur += nappend;
            return nappend;
        }
    }

    void FixedBuffer::cookieStart()
    {

    }

    void FixedBuffer::cookieEnd()
    {

    }


}
}
