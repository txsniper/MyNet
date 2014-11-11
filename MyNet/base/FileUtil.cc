#include "FileUtil.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
namespace MyNet
{
namespace base
{
    ReadSmallFile::ReadSmallFile(const std::string& filename)
        : m_filename(filename),
          m_fd(::open(filename.c_str(), O_RDONLY | O_CLOEXEC)),
          m_err(0),
          m_size(0)
    {
        m_buff[0] = '\0';
        if(m_fd < 0)
            m_err = errno;
    }

    ReadSmallFile::~ReadSmallFile()
    {
        int ret;
        if(m_fd >= 0)
        {
           do
           {
               // should check EINTR
               ret = ::close(m_fd);
           }while((ret == -1) && (errno == EINTR) );

        }
    }

    int ReadSmallFile::readToBuffer(int *size)
    {
        int err = m_err;
        if(m_fd >= 0)
        {
            ssize_t ret = 0;
            do
            {
                ret = ::pread(m_fd, m_buff, sizeof(m_buff) - 1, 0);
            } while((ret == -1) && (errno = EINTR));
            if(ret >= 0)
            {
                m_buff[ret] = '\0';
                if(size)
                {
                    *size = static_cast<int> (ret);
                }
            }
            else
            {
                err = errno;
            }
        }
        return err;
    }

    int ReadSmallFile::readToString(int64_t maxSize, std::string& data,
                                    int64_t* fileSize, int64_t* modifyTime,
                                    int64_t* createTime)
    {
        assert(sizeof(off_t) == 8);
        int err = m_err;
        int64_t readSize = 0;
        if(m_fd >= 0)
        {
            data.clear();
            struct stat fileStat;
            int ret = ::fstat(m_fd, &fileStat);
            if(ret == 0)
            {
                //success
                if(S_ISREG(fileStat.st_mode) && fileSize)
                {
                    *fileSize = fileStat.st_size;
                    readSize = std::min(maxSize, fileStat.st_size);
                    data.reserve(readSize);
                }
                else if(S_ISDIR(fileStat.st_mode))
                {
                    err = EISDIR;
                }
                if(modifyTime)
                {
                    *modifyTime = fileStat.st_mtime;
                }
                if(createTime)
                {
                    *createTime = fileStat.st_ctime;
                }
            }
            else
            {
                err = errno;
            }
        }
        int64_t readed = 0;

        if(readSize > 0)
        {
            while(readed < readSize)
            {
                size_t need_read = std::min(sizeof(m_buff), maxSize - data.size());
                ssize_t n = ::read(m_fd, m_buff, need_read);
                if(n > 0)
                {
                    readed += n;
                    data.append(m_buff, n);
                }
                else if(n == 0)
                {
                    break;
                }
                else
                {
                    if(errno != EINTR)
                    {
                        err = errno;
                        break;
                    }
                }
            }
        }
        return err;
    }


    WriteFile::WriteFile(const std::string& filename)
        : m_filename(filename),
          m_file(::fopen(filename.c_str(), "ae")),
          m_nBytesWritten(0),
          m_valid(true)
    {
        if(m_file == NULL)
            m_valid = false;
        else
        {
            ::setbuffer(m_file, m_buff, sizeof(m_buff));
        }
    }

    WriteFile::~WriteFile()
    {
        if(m_valid)
        {
            ::fclose(m_file);
        }
    }

    void WriteFile::flush()
    {
        if(m_valid)
        {
            ::fflush(m_file);
        }
    }
    size_t WriteFile::writeLine(const std::string& line)
    {
        return writeLine(line.c_str(), line.size());
    }

    size_t WriteFile::writeLine(const char* line, size_t n)
    {
        size_t ret = 0;
        size_t remain = n;
        size_t startPos = 0;
        if(m_valid)
        {
            do
            {
                ret = write(line + startPos, remain);
                if(ret == 0)
                {
                    int err = ::ferror(m_file);
                    if(err)
                    {
                        // error occured
                        ::fprintf(stderr, "WriteFile error: %s\n", ::strerror(err));
                        break;
                    }
                }
                remain = remain - ret;
                startPos += ret;
            } while(remain > 0);
            m_nBytesWritten += n;
            return n;
        }
        else
        {
            return 0;
        }
    }

    size_t WriteFile::write(const char* line, size_t n)
    {
        return ::fwrite_unlocked(line, n, 1, m_file);
    }

}
}
