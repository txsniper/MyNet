#include "Connector.h"
#include "Channel.h"
#include "SocketsOps.h"
#include "EventLoop.h"

#include <boost/bind.hpp>
#include <errno.h>

using namespace MyNet;
using namespace MyNet::net;

const int Connector::kMaxRetryDelayMs;

    Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
: loop_(loop),
    serverAddr_(serverAddr),
    connect_(false),
    state_(kDisconnected),
    retryDelayMs_(kInitRetryDelayMs)
{
    LOG_DEBUG << "ctor[" << this << "]";
}

Connector::~Connector()
{
    LOG_DEBUG << "dtor[" << this << "]";
    assert(!channel_);
}

void Connector::start()
{
    connect_ = true;
    loop_->runInLoop(boost::bind(&Connector::startInLoop, this));
}

void Connector::startInLoop()
{
    loop_->assertInLoopThread();
    assert(state_ == kDisconnected);
    if (connect_)
    {
        connect();
    }
    else
    {
        LOG_DEBUG << "do not connect";
    }
}

void Connector::stop()
{
    connect_ = false;
    loop_->queueInLoop(boost::bind(&Connector::stopInLoop, this));
}

void Connector::stopInLoop()
{
    loop_->assertInLoopThread();
    if (state_ == kConnected)
    {
        setState(kDisconnected);
        int sockfd = removeAndResetChannel();
        retry(sockdf);
    }
}

void Connector::connect()
{
    int sockfd = sockets::createNonBlockingOrDie();
    int ret = sockets::connect(sockfd, serverAddr_.getSockAddrInet());
    int savedErrno = (ret == 0) ? 0 : errno;
    switch(savedErrno)
    {
        case 0:
        case EINPROGRESS:
        case EINTR:
        case EISCONN:
            connecting(sockfd);
            break;
        case EAGAIN:
        case EADDRINUSE:
        case EADDRNOTAVAIL:
        case ECONNREFUSED:
        case ENETUNREACH:
            retry(sockfd);
            break;

        case EACCES:
        case EPERM:
        case EAFNOSUPPORT:
        case EALREADY:
        case EBADF:
        case EFAULT:
        case ENOTSOCK:
            LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
            sockets::close(sockfd);
            break;

        default:
            LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
            sockets::close(sockfd);
            // connectErrorCallback_();
            break;
    }
}
