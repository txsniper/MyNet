#ifndef _MYNET_BASE_EXCEPTION_H_
#define _MYNET_BASE_EXCEPTION_H_

#include <execinfo.h>
#include <string>

namespace MyNet
{

namespace base
{

// 在C++11中，声明一个函数不抛出异常使用noexcept，在此之前实现
// 相同的功能使用throw() (muduo使用它)
class Exception : public std::exception
{
    public:
        explicit Exception(const std::string& what);
        explicit Exception(const char* what);
        virtual ~Exception() throw();
        virtual const char* what() const throw();
        const char* stackTrace() const throw();

    private:
        void fillStackTrack();

        std::string message_;
        std::string stack_;

};
}
}
#endif

