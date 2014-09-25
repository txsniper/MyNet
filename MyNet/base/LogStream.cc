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
