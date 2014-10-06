#include "ProcessInfo.h"
#include "FileUtil.h"
#include "ThreadOp.h"
#include <algorithm>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/times.h>
namespace MyNet
{

    // 获取进程启动时的信息，启动时间(g_startTime)
namespace ProcInfo
{
    // 获取当前打开的文件数量
    __thread int t_numOpenedFiles = 0;
    int fdDirFilter(const struct dirent* d)
    {
        if(::isdigit(d->d_name[0]))
        {
            ++t_numOpenedFiles;
        }
        return 0;
    }

    // 运用一个线程私有变量(t_pids)并不是必须 
    __thread std::vector<pid_t>* t_pids = NULL;
    int taskDirFilter(const struct dirent* d)
    {
        if(::isdigit(d->d_name[0]))
        {
            t_pids->push_back(::atoi(d->d_name));
        }
        return 0;
    }

    int scanDir(const char* dirpath, int (*filter)(const struct dirent* ))
    {
        struct dirent** namelist = NULL;
        int result = ::scandir(dirpath, &namelist, filter, ::alphasort);
        assert(namelist == NULL);
        return result;
    }

    MyNet::base::Timestamp g_StartTime = MyNet::base::Timestamp::now();

    int g_clockTicks = static_cast<int>(::sysconf(_SC_CLK_TCK));
    int g_pageSize = static_cast<int>(::sysconf(_SC_PAGE_SIZE));

    pid_t pid()
    {
        return ::getpid();
    }

    std::string pidStrng()
    {
        char buf[32] = {0};
        ::snprintf(buf, sizeof(buf), "%d", pid());
        return buf;
    }

    uid_t uid()
    {
        return ::getuid();
    }

    std::string username()
    {
        struct passwd pwd;
        struct passwd* result = NULL;
        char buf[8192] = {0};
        const char* name = "unknownuser";

        ::getpwuid_r(uid(), &pwd, buf, sizeof(buf), &result);
        if(result)
        {
            name = result->pw_name;
        }
        return name;
    }

    uid_t euid()
    {
        return ::geteuid();
    }

    // 在进程启动的时候确定startTime, clockTicks, pageSize
    MyNet::base::Timestamp startTime()
    {
        return g_StartTime;
    }

    int clockTicksPerSecond()
    {
        return g_clockTicks;
    }

    int pagesize()
    {
        return g_pageSize;
    }

    bool isDebugBuild()
    {
#ifdef NDEBUG
        return false;
#else
        return true;
#endif
    }

    std::string procname()
    {
        return procname(procStat());
    }
    std::string procname(const std::string& stat)
    {
        std::string name;
        size_t lp = stat.find('(');
        size_t rp = stat.find(')');
        if(lp != std::string::npos && rp != std::string::npos && lp < rp)
        {
            // TODO 运用别的方法赋值
            name = std::string(stat.data() + lp + 1, static_cast<int>(rp - lp -1 ));
        }
        return name;
    }

    std::string procStatus()
    {
        std::string result;
        MyNet::base::readFile("/proc/self/status", 65536, &result);
        return result;
    }

    std::string procStat()
    {
        std::string result;
        MyNet::base::readFile("/proc/self/stat", 65536, &result);
        return result;
    }

    std::string threadStat()
    {
        char buf[64];
        ::snprintf(buf, sizeof(buf), "/proc/self/task/%d/stat", MyNet::ThreadOp::tid());
        std::string result;
        MyNet::base::readFile(buf, 65536, &result);
        return result;
    }

    std::string exePath()
    {
        std::string result;
        char buf[1024] = {0};
        ssize_t n = ::readlink("/proc/self/exe", buf, sizeof(buf));
        if( n > 0)
        {
            result.assign(buf, n);
        }
        return result;
    }

    int openedFiles()
    {
        t_numOpenedFiles = 0;
        scanDir("/proc/self/fd", fdDirFilter);
        return t_numOpenedFiles;
    }

    int maxOpenFiles()
    {
        struct rlimit rl;
        if(::getrlimit(RLIMIT_NOFILE, &rl))
        {
            return openedFiles();
        }
        else
        {
            return static_cast<int>(rl.rlim_cur);
        }
    }

    CpuTime cputime()
    {
        CpuTime t;
        struct tms tms;
        if(::times(&tms) >= 0)
        {
            const double hz = static_cast<double>(clockTicksPerSecond());
            t.userSeconds = static_cast<double>(tms.tms_utime) / hz;
            t.systemSeconds = static_cast<double>(tms.tms_stime) / hz;
        }
        return t;
    }

    int numThreads()
    {
        int result = 0;
        std::string status = procStatus();
        size_t pos = status.find("Threads:");
        if(pos != std::string::npos)
        {
            result = ::atoi(status.c_str() + pos + 8);
        }
        return result;
    }

    std::vector<pid_t> threads()
    {
        std::vector<pid_t> result;
        t_pids = &result;
        scanDir("/proc/self/task", taskDirFilter);
        t_pids = NULL;
        std::sort(result.begin(), result.end());
        return result;
    }






}
}
