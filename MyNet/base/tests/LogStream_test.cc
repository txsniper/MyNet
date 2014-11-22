#include "../LogStream.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <limits.h>
#include <stdint.h>
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
using namespace MyNet::base;


/*
int main()
{
    const int size = 10;
    const char* data = "123456";
    FixedBuffer<size> buffer;
    size_t ret = buffer.append(data, ::strlen(data));
    ::printf("ret=%zd, data=%s\n", ret, buffer.data());
    ret = buffer.appendPartial(data, ::strlen(data));
    ::printf("ret=%zd, data=%s\n", ret, buffer.data());
    ret = buffer.appendPartial(data, ::strlen(data));
    ::printf("ret=%zd, data=%s\n", ret, buffer.data());
    std::string str = buffer.asString();
    ::printf("str=%s\n", str.c_str());

}
*/

BOOST_AUTO_TEST_CASE(testFixedBuffer)
{
    const int size = 10;
    const char* data="123456";
    FixedBuffer<size> buffer;
    BOOST_CHECK_EQUAL(buffer.asString(), std::string(""));
    size_t ret = buffer.append(data, ::strlen(data));
    BOOST_CHECK_EQUAL(ret, ::strlen(data));
    BOOST_CHECK_EQUAL(buffer.asString(), std::string(data));
    ret = buffer.append(data, ::strlen(data));
    BOOST_CHECK_EQUAL(ret, 0);
    BOOST_CHECK_EQUAL(buffer.asString(), std::string(data));
    ret = buffer.appendPartial(data, ::strlen(data));
    BOOST_CHECK_EQUAL(ret, 4);
    ret = buffer.appendPartial(data, ::strlen(data));
    BOOST_CHECK_EQUAL(ret, 0);
    BOOST_CHECK_EQUAL(buffer.length(), size);
}

BOOST_AUTO_TEST_CASE(testLogStreamBooleans)
{
    LogStream os;
    const LogStream::Buffer& os_buffer = os.buffer();
    BOOST_CHECK_EQUAL(os_buffer.asString(), std::string(""));
    os << true;
    BOOST_CHECK_EQUAL(os_buffer.asString(), std::string("1"));
    os << false;
    BOOST_CHECK_EQUAL(os_buffer.asString(), std::string("10"));
    os << '\n';
    BOOST_CHECK_EQUAL(os_buffer.asString(), std::string("10\n"));
    os << true;
    BOOST_CHECK_EQUAL(os_buffer.asString(), std::string("10\n1"));
}

BOOST_AUTO_TEST_CASE(testLogStreamInteger)
{
    LogStream os;
    const LogStream::Buffer& os_buffer = os.buffer();
    os << 123;
    BOOST_CHECK_EQUAL(os_buffer.asString(), std::string("123"));
    os << ' '<<123.45;
    BOOST_CHECK_EQUAL(os_buffer.asString(), std::string("123 123.45"));
    os <<-10.2;
    BOOST_CHECK_EQUAL(os_buffer.asString(), std::string("123 123.45-10.2"));
    os.resetBuffer();
    os <<10<<"-"<<-20;
    BOOST_CHECK_EQUAL(os.buffer().asString(), std::string("10--20"));
    // This is an ERROR test
    BOOST_CHECK_EQUAL(1, 2);
}

BOOST_AUTO_TEST_CASE(testLogStreamIntegerLimit)
{
    LogStream os;
    const LogStream::Buffer& os_buffer = os.buffer();
    os << INT_MAX;
    BOOST_CHECK_EQUAL(::atoi(os_buffer.asString().c_str()), INT_MAX);
    os.resetBuffer();
    os << INT_MIN;
    BOOST_CHECK_EQUAL(::atoi(os_buffer.asString().c_str()), INT_MIN);
    os.resetBuffer();
    os << LLONG_MAX;
    BOOST_CHECK_EQUAL(::atoll(os_buffer.asString().c_str()), LLONG_MAX);

}

BOOST_AUTO_TEST_CASE(testLogStreamPtr)
{
    char *ptr = NULL;
    LogStream os;
    const LogStream::Buffer& os_buffer = os.buffer();
    os << static_cast<void *> (ptr);
    BOOST_CHECK_EQUAL(os_buffer.asString(), std::string("0x0"));
}
