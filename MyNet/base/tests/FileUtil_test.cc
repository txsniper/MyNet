#include "../FileUtil.h"
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

int main()
{
    std::string data;
    int64_t size = 0;
    int err = MyNet::base::readFile("/proc/self", 1024, &data, &size);
    ::printf("%d %zd %" PRIu64 "\n", err, data.size(), size);
    err = MyNet::base::readFile("/proc/self", 1024, &data, NULL);
    ::printf("%d %zd %" PRIu64 "\n", err, data.size(), size);
    err = MyNet::base::readFile("/proc/self/syscall", 1024, &data, &size);
    ::printf("%d %zd %" PRIu64 "\n", err, data.size(), size);
    err = MyNet::base::readFile("/proc/self/cmdline", 1024, &data, &size);
    ::printf("%d %zd %" PRIu64 "\n", err, data.size(), size);
    err = MyNet::base::readFile("/dev/zero", 102400, &data, NULL);
    ::printf("%d %zd %" PRIu64 "\n", err, data.size(), size);
    err = MyNet::base::readFile("/home/sniper/github/MyNet/MyNet/base/Thread.h", 1024, &data, &size);
    ::printf("%d %zd %" PRIu64 "\n", err, data.size(), size);
    err = MyNet::base::readFile("/home/sniper/github/MyNet/MyNet/base/Thread.h", 102400, &data, &size);
    ::printf("%d %zd %" PRIu64 "\n", err, data.size(), size);
}
