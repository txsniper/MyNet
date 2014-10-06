#ifndef _BASE_PROCESS_INFO_H_
#define _BASE_PROCESS_INFO_H_
#include "Timestamp.h"

#include <string>
#include <vector>
namespace MyNet
{

namespace ProcInfo
{
    pid_t pid();
    std::string pidString();
    uid_t uid();
    std::string username();
    uid_t euid();
    MyNet::base::Timestamp startTime();
    int clockTicksPerSecond();
    int pagesize();
    bool isDebugBuild();

    std::string hostname();
    std::string procname();
    std::string procname(const std::string& stat);


    std::string procStatus();
    std::string procStat();
    std::string threadStat();
    std::string exePath();

    int openedFiles();
    int maxOpenFiles();

    struct CpuTime
    {
        double userSeconds;
        double systemSeconds;
        CpuTime()
            : userSeconds(0.0), systemSeconds(0.0)
        {

        }
    };

    CpuTime cputime();
    int numThreads();
    std::vector<pid_t> threads();

}
}
#endif
