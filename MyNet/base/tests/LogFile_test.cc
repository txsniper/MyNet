#include "../LogFile.h"
#include "../Logging.h"
#include <boost/scoped_ptr.hpp>

boost::scoped_ptr<MyNet::base::LogFile> g_logfile;

void outputFunc(const char* logline, size_t len)
{
    g_logfile->append(logline, len);
}

void flushFunc()
{
    g_logfile->flush();
}

int main(int argc, char * argv[])
{
    char name[256];
    ::strncpy(name, argv[0], sizeof(name));
    g_logfile.reset(new MyNet::base::LogFile(true, 10 * 1024, ::basename(name)));
    MyNet::base::Logger::setOutput(outputFunc);
    MyNet::base::Logger::setFlush(flushFunc);
    std::string line = "0123456789abcdefghijklmnopqrstuvwxyz";
    for(int i=0; i<10*10000; ++i)
    {
        LOG_INFO << line << i;
        ::usleep(100);
    }
    return 0;
}
