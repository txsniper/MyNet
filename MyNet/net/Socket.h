#ifndef _MYNET_NET_SOCKET_H_
#define _MYNET_NET_SOCKET_H_

#include "../base/NonCopyable.h"

namespace MyNet
{
namespace net
{
    class InetAddress;

    class Socket : MyNet::Noncopyable
    {
        public:
            explicit Socket(int sockfd)
                : sockfd_(sockfd)
            {

            }

            ~Socket();

            int fd() const
            {
                return sockfd_;
            }

            // bind local address or die
            void bindAddress(const InetAddress& localaddr);

            void listen();

            // return socket and set peeraddr
            int accept(InetAddress* peeraddr);

            void shutdownWrite();

            // enable/disable TCP_NODELAY
            void setTcpNoDelay(bool on);

            // enable/disable SO_REUSEADDR
            void setReuseAddr(bool on);

            // enable/disable SO_REUSEPORT
            void setReusePort(bool on);

            //  Enable/disable SO_KEEPALIVE
            void setKeepAlive(bool on);

        private:
            const int sockfd_;
    };
}
}
#endif
