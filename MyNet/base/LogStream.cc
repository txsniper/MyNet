#include <algorithm>
#include <stdint.h>
#include <stdio.h>
#include "LogStream.h"

//必须忽略-Wtype-limits，否则当把convert函数的模板参数T
//具体化为无符号类型时，if(value < 0)会报warnning:
//错误： 无符号表达式永远不小于 0 [-Werror=type-limits]
#pragma GCC diagnostic ignored "-Wtype-limits"

namespace MyNet
{
namespace base
{

    const char digits[] = "9876543210123456789";
    const char* zero = digits + 9;
    const char digitsHex[] = "0123456789ABCDEF";

    template<typename T>
    size_t convert(char buf[], T value)
    {
        T i = value;
        char* p = buf;
        do
        {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);

        if (value < 0)
        {
            *p++ = '-';
        }
        *p = '\0';
        std::reverse(buf, p);
        return p - buf;
    }

    size_t convertHex(char buf[], uintptr_t value)
    {
      uintptr_t i = value;
      char* p = buf;
      do
      {
          int lsd = i % 16;
          i /= 16;
          *p++ = digitsHex[lsd];
      } while (i != 0);
      *p = '\0';
      std::reverse(buf, p);
      return p - buf;
    }


    template <typename T>
    void LogStream::formatInteger(T value)
    {
        if(m_buff.avail() >= kMaxNumericSize)
        {
            size_t len = convert(m_buff.current(), value);
            m_buff.add(len);
        }
    }

    size_t LogStream::append(const char* line, size_t len)
    {
        return m_buff.append(line, len);
    }

    LogStream& LogStream::operator<< (const std::string& line)
    {
        m_buff.append(line.c_str(), line.size());
        return *this;
    }

    LogStream& LogStream::operator<< (const char* line)
    {
        m_buff.append(line, strlen(line));
        return *this;
    }

    LogStream& LogStream::operator<< (bool b)
    {
        char c = (b?'1':'0');
        m_buff.append(&c, 1);
        return *this;
    }

    LogStream& LogStream::operator<< (char c)
    {
        m_buff.append(&c, 1);
        return *this;
    }

    LogStream& LogStream::operator<< (unsigned char uc)
    {
        char c = static_cast<char>(uc);
        *this << c;
        return *this;
    }

    LogStream& LogStream::operator<< (short s)
    {
        *this << static_cast<int>(s);
        return *this;
    }

    LogStream& LogStream::operator<< (unsigned short us)
    {
        *this << static_cast<unsigned int>(us);
        return *this;
    }

    LogStream& LogStream::operator<< (int i)
    {
        formatInteger(i);
        return *this;
    }

    LogStream& LogStream::operator<< (unsigned int ui)
    {
        formatInteger(ui);
        return *this;
    }

    LogStream& LogStream::operator<< (long l)
    {
        formatInteger(l);
        return *this;
    }

    LogStream& LogStream::operator<<(long long ll)
    {
        formatInteger(ll);
        return *this;
    }

    LogStream& LogStream::operator<< (unsigned long ul)
    {
        formatInteger(ul);
        return *this;
    }

    LogStream& LogStream::operator<< (unsigned long long ull)
    {
        formatInteger(ull);
        return *this;
    }

    LogStream& LogStream::operator<< (float f)
    {
        *this << f;
        return *this;
    }

    LogStream& LogStream::operator<< (double d)
    {
        if(m_buff.avail() >= kMaxNumericSize)
        {
            int len = snprintf(m_buff.current(), kMaxNumericSize, "%.12g", d);
            m_buff.add(len);
        }
        return *this;
    }

    LogStream& LogStream::operator<< (const void* p)
    {
        uintptr_t t = reinterpret_cast<uintptr_t>(p);
        if(m_buff.avail() >= kMaxNumericSize)
        {
            m_buff.append("0x", 2);
            size_t len = convertHex(m_buff.current(), t);
            m_buff.add(len);
        }
        return *this;
    }


}
}
