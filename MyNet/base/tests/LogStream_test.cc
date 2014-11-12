#include "../LogStream.h"
#include <stdio.h>
#include <string.h>
using namespace MyNet::base;

int main()
{
    const int size = 10;
    const char* data = "abcde123456789";
    FixedBuffer<size> buffer;
    size_t ret = buffer.append(data, ::strlen(data));
    ::printf("ret=%zd, data=%s", ret, buffer.data());
    ret = buffer.appendPartial(data, ::strlen(data));
    ::printf("ret=%zd, data=%s", ret, buffer.data());

}

