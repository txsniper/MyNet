#ifndef _MYNET_NET_SOCKETS_OPS_
#define _MYNET_NET_SOCKETS_OPS_

#include <arpa/inet.h>
namespace MyNet
{
namespace net
{

namespace sockets
{
    int createNonBlockingOrDie();
    int connect(int sockfd, const struct sockaddr_in& addr);
    void bindOrDie(int sockfd, const struct sockaddr_in& addr);
    void listenOrDie(int sockfd);
    int accept(int sockfd, struct sockaddr_in* addr);
    ssize_t read(int sockfd, void* buf, size_t count);
    ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
    ssize_t write(int sockfd, const void* buf, size_t count);
    void close(int sockfd);
    void shutdownWrite(int sockfd);

    // char* to sockaddr and sockaddr to char*
    void toIpPort(char* buf, size_t size, const struct sockaddr_in& addr);

    void toIp(char* buf, size_t size, const struct sockaddr_in& addr);

    void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

    int getSocketError(int sockfd);

    // 网络地址类型转换
    const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
    struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);

    const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
    struct sockaddr_in* sockaddr_in_cast(struct sockaddr* addr);

    struct sockaddr_in getLocalAddr(int sockfd);
    struct sockaddr_in getPeerAddr(int sockfd);
    bool isSelfConnect(int sockfd);
}
}
}
#endif
