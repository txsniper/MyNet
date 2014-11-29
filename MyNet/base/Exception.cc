#include "Exception.h"
#include <stdlib.h>
namespace MyNet
{

namespace base
{

Exception::Exception(const std::string& what_str)
    : message_(what_str)
{
    fillStackTrack();
}

Exception::Exception(const char* what_str)
    : message_(what_str)
{
    fillStackTrack();
}

Exception::~Exception() throw()
{

}

const char* Exception::what() const throw()
{
    return message_.c_str();
}

const char* Exception::stackTrace() const throw()
{
    return stack_.c_str();
}

void Exception::fillStackTrack()
{
    const int len = 200;
    void *buff[len];
    int ret = ::backtrace(buff, len);
    char **strings = ::backtrace_symbols(buff, ret);
    if (strings)
    {
        for(int i=0; i<ret; ++i)
        {
            stack_.append(strings[i]);
            stack_.push_back('\n');
        }
    }
    ::free(strings);
}
}

}
