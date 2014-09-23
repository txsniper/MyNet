#ifndef _BASE_FILE_UTIL_H_
#define _BASE_FILE_UTIL_H_
#include "NonCopyable.h"
#include <string.h>
#include <stdio.h>
namespace MyNet
{
namespace base
{
    class ReadSmallFile : private MyNet::Noncopyable
    {
        public:
            ReadSmallFile(const std::string& filename);
            ~ReadSmallFile();
            int readToBuffer(int *size);
            int readToString(std::string& data, int64_t maxSize, int64_t *fileSize,
                             int64_t *modifyTime, int64_t* createTime);
            const char* buffer() {return m_buff;}
        public:
            const static int kBUFF_SIZE = 64 * 1024;
        private:
            std::string m_filename;
            int m_fd;
            int m_err;
            size_t m_size;
            char m_buff[kBUFF_SIZE];

    };

    class WriteFile: private MyNet::Noncopyable
    {
        public:
            WriteFile(const std::string& filename);
            ~WriteFile();
            size_t writeLine(const std::string& line);
            size_t writeLine(const char* line, size_t n);
            size_t size() const {return m_nBytesWritten;}
            bool valid() const { return m_valid;}
            void flush();
        private:
            size_t write(const char* line, size_t n);
        public:
            const static kBUFF_SIZE = 64 * 1024;
        private:
            std::string m_filename;
            FILE* m_file;
            size_t m_nBytesWritten;
            bool m_valid;
            char m_buff[kBUFF_SIZE];
    }
}
}

endif
