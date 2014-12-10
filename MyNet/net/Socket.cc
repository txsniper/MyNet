#include "Socket.h"
#include "SocketsOps.h"
#include "InetAddress.h"

// get TCP_NODELAY
#include <netinet/tcp.h>

// get bzero
#include <string.h>
namespace MyNet
{
namespace net
{

Socket::~Socket()
{
    sockets::close(sockfd_);
}

void Socket::bindAddress(const InetAddress& localaddr)
{
    sockets::bindOrDie(sockfd_, localaddr.getSockAddrInet());
}

void Socket::listen()
{
    sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress* peeraddr)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof addr);
    int connfd = sockets::accept(sockfd_, &addr);
    if (connfd > 0)
    {
        peeraddr->setSockAddrInet(addr);
    }
    return connfd;
}

void Socket::shutdownWrite()
{
    sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on)
{
    int flag = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &flag, static_cast<socklen_t>(sizeof flag));
    if (ret < 0)
    {
        // ERROR
    }
}

void Socket::setKeepAlive(bool on)
{
    int flag = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &flag, static_cast<socklen_t>(sizeof flag));
    if (ret < 0)
    {
        //ERROR
    }
}

void Socket::setReuseAddr(bool on)
{
    int flag = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &flag, static_cast<socklen_t>(sizeof flag));
    if (ret < 0)
    {
        //ERROR
    }
}

void Socket::setReusePort(bool on)
{
    int flag = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &flag, static_cast<socklen_t>(sizeof flag));
    if (ret < 0)
    {
        //ERROR
    }
}

}
}
