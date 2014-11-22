#include "AsyncLogging.h"
#include "LogFile.h"
#include "Timestamp.h"

#include <boost/bind.hpp>

namespace MyNet
{
namespace base
{
    AsyncLogging::AsyncLogging(const std::string& basename, size_t rollSize, int flushInterval)
        : m_rollSize(rollSize),
          m_flushInterval(flushInterval),
          m_basename(basename),
          m_running(false),
          m_loggingThread(boost::bind(&AsyncLogging::ThreadFunc, this), "LoggingThread"),
          m_latch(1),
          m_mutex(),
          m_cond(m_mutex),
          m_curbuff(new Buffer()),
          m_nextbuff(new Buffer()),
          m_buffs()
    {
        m_curbuff->bzero();
        m_nextbuff->bzero();
        m_buffs.reserve(16);
    }

    void AsyncLogging::start()
    {
        m_running = true;
        m_loggingThread.start();
        m_latch.wait();
    }

    void AsyncLogging::stop()
    {
        m_running = false;
        m_loggingThread.join();
    }

    void AsyncLogging::append(const char* logline, size_t length)
    {
        MutexLockGuard lock(m_mutex);

        if (m_curbuff->avail() > length)
        {
            m_curbuff->append(logline, length);
        }
        else
        {
            m_buffs.push_back(m_curbuff.release());
            if (m_nextbuff)
            {
                m_curbuff = boost::ptr_container::move(m_nextbuff);
            }
            else
            {
                m_curbuff->reset();
            }
            m_curbuff->append(logline, length);
            m_cond.notify();
        }
    }
    void AsyncLogging::ThreadFunc()
    {
        assert(m_running == true);
        m_latch.countDown();
        LogFile logout(true, m_rollSize, m_basename, m_flushInterval);
        BuffPtr firstBuffer(new Buffer());
        BuffPtr secondBuffer(new Buffer());
        BuffVec buffsToWrite;
        buffsToWrite.reserve(16);

        while(m_running)
        {
            {
                MutexLockGuard lock(m_mutex);
                // 如果前端的日志块缓冲区中为空，则等待一段时间
                if (m_buffs.empty())
                {
                    m_cond.waitForSeconds(m_flushInterval);
                }
                // 将前端的当前缓冲区转移到m_buffs中
                m_buffs.push_back(m_curbuff.release());

                // 将后端的两个缓冲区和前端的交换，并且交换缓冲区列表
                m_curbuff = boost::ptr_container::move(firstBuffer);
                buffsToWrite.swap(m_buffs);

                // m_nextbuff为空，说明前端预分配的两个缓冲区已经至少用完了一个
                if (!m_nextbuff)
                {
                    m_nextbuff = boost::ptr_container::move(secondBuffer);
                }
            }
            assert(buffsToWrite.size() > 0);

            // 日志量太大，需要丢弃一些日志，并且释放内存块
            if (buffsToWrite.size() > 25)
            {
                char temp[256];
                ::snprintf(temp, sizeof(temp), "Drop some log at %s, %zd", Timestamp::now().toFormatedString().c_str(), buffsToWrite.size() - 2);
                ::fputs(temp, stderr);
                logout.append(temp, sizeof(temp));
                buffsToWrite.erase(buffsToWrite.begin() + 2, buffsToWrite.end());
            }

            for(size_t i=0; i<buffsToWrite.size(); ++i)
            {
                logout.append(buffsToWrite[i].data(), buffsToWrite[i].length());
            }

            // resize将销毁多余的元素
            if (buffsToWrite.size() > 2)
            {
                buffsToWrite.resize(2);
            }

            if (!firstBuffer)
            {
                // buffsToWrite不会为空
                assert(!buffsToWrite.empty());
                firstBuffer = buffsToWrite.pop_back();
                firstBuffer->reset();
            }

            if (!secondBuffer)
            {
                // 在secondBuffer为空的情况下，buffsToWrite至少
                // 包含两个元素
                assert(!buffsToWrite.empty());
                secondBuffer = buffsToWrite.pop_back();
                secondBuffer->reset();
            }

            // clear只是清除元素，不释放内存
            buffsToWrite.clear();
            logout.flush();
        }
        logout.flush();

    }
}
}
