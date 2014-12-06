#include "SocketsOps.h"
#include "Endian.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <sys/uio.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <boost/implicit_cast.hpp>
using boost::implicit_cast;
namespace MyNet
{
namespace net
{

namespace sockets
{
#if VALGRIND || defined (NO_ACCEPT4)
void setNonBlockAndCloseOnExec(int sockfd)
{
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd, F_SETFL, flags);
    flags = ::fcntl(sockfd, F_GETFD, 0);
    flags |= CLOEXEC;
    ret = ::fcntl(sockfd, F_SETFD, flags);
    (void) ret;
}
#endif

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr)
{
    return static_cast<const struct sockaddr*>(implicit_cast<const void*>(addr));
}

struct sockaddr* sockaddr_cast(struct sockaddr_in* addr)
{
    return static_cast<struct sockaddr*>(implicit_cast<void*>(addr));
}

const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr)
{
    return static_cast<const struct sockaddr_in*>(implicit_cast<const void*>(addr));
}

struct sockaddr_in* sockaddr_in_cast(struct sockaddr* addr)
{
    return static_cast<struct sockaddr_in*>(implicit_cast<void *>(addr));
}

int createNonBlockingOrDie()
{
#if VALGRIND
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        // ERROR
    }
    setNonBlockAndCloseOnExec(sockfd);
#else
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        // ERROR
    }
#endif
    return sockfd;
}

void bindOrDie(int sockfd, const struct sockaddr_in& addr)
{
    int ret = ::bind(sockfd, sockaddr_cast(&addr), static_cast<socklen_t>(sizeof addr));
    if (ret < 0)
    {
        // ERROR
    }
}


void listenOrDie(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    if (ret < 0)
    {
        //ERROR
    }
}

int accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof(struct sockaddr_in));
#if VALGRIND || defined(NO_ACCEPT4)
    int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
    setNonBlockAndCloseOnExec(connfd);
#else
    int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
    if (connfd < 0)
    {
        // ERROR
        int savedErrno = errno;
        switch(savedErrno)
        {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO:
            case EPERM:
            case EMFILE:
                errno = savedErrno;
                break;
            // TODO : abort
        }
    }
    return connfd;
}

int connect(int sockfd, const struct sockaddr_in& addr)
{
    return ::connect(sockfd, sockaddr_cast(&addr), static_cast<socklen_t>(sizeof addr));
}

ssize_t read(int sockfd, void *buf, size_t count)
{
    return ::read(sockfd, buf, count);
}

ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    return ::readv(sockfd, iov, iovcnt);
}

ssize_t write(int sockfd, const void* buf, size_t count)
{
    return ::write(sockfd, buf, count);
}

void close(int sockfd)
{
    int ret = ::close(sockfd);
    if (ret < 0)
    {
        //ERROR
    }
}

void shutdownWrite(int sockfd)
{
    int ret = ::shutdown(sockfd, SHUT_WR);
    if (ret < 0)
    {
        //ERROR
    }
}

void toIpPort(char *buf, size_t size, const struct sockaddr_in& addr)
{
    assert( size >= INET_ADDRSTRLEN);
    ::inet_ntop(AF_INET, sockaddr_cast(&addr), buf, static_cast<socklen_t>(size));
    size_t end = ::strlen(buf);
    uint16_t port = networkToHost16(addr.sin_port);
    assert( size > end);
    ::snprintf(buf+end, size-end, ":%u", port);
}

void toIp(char *buf, size_t size, const struct sockaddr_in& addr)
{
    assert(size > INET_ADDRSTRLEN);
    ::inet_ntop(AF_INET, sockaddr_cast(&addr), buf, static_cast<socklen_t>(size));

}

void fromIpPort(const char *ip, uint16_t port, struct sockaddr_in *addr)
{
    addr->sin_family = AF_INET;
    addr->sin_port = hostToNetwork16(port);
    int ret = ::inet_pton(AF_INET, ip, &addr->sin_addr);
    if (ret <= 0)
    {
        //ERROR
    }
}

int getSocketError(int sockfd)
{
    int optval;
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);
    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
        return errno;
    }
    else
    {
        return optval;
    }
}

struct sockaddr_in getLocalAddr(int sockfd)
{
    struct sockaddr_in local_addr;
    ::bzero(&local_addr, sizeof local_addr);
    socklen_t len  = static_cast<socklen_t>(sizeof local_addr);
    int ret = ::getsockname(sockfd, sockaddr_cast(&local_addr), &len);
    if (ret < 0)
    {
        // ERROR
    }
    return local_addr;
}

struct sockaddr_in getPeerAddr(int sockfd)
{
    struct sockaddr_in peer_addr;
    ::bzero(&peer_addr, sizeof(peer_addr));
    socklen_t len = static_cast<socklen_t>(sizeof peer_addr);
    int ret = ::getpeername(sockfd, sockaddr_cast(&peer_addr), &len);
    if (ret < 0)
    {
        //ERROR
    }
    return peer_addr;
}

bool isSelfConnect(int sockfd)
{
    sockaddr_in local = getLocalAddr(sockfd);
    sockaddr_in peer = getPeerAddr(sockfd);
    return local.sin_addr.s_addr == peer.sin_addr.s_addr && local.sin_port == peer.sin_port;

}
}
}
}
