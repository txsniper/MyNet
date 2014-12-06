#ifndef _MYNET_NET_INETADDRESS_
#define _MYNET_NET_INETADDRESS_

#include "../base/Copyable.h"
#include <string>
#include <netinet/in.h>

using std::string;

namespace MyNet
{
namespace net
{
    class InetAddress : public MyNet::Copyable
    {
        public:
            InetAddress(const std::string& ip_addr, uint16_t port);
            InetAddress(uint16_t port, bool loopbackonly = false);
            InetAddress(const struct sockaddr_in& addr)
                : addr_(addr)
            {

            }

            const struct sockaddr_in& getSockAddrInet() const
            {
                return addr_;
            }

            void setSockAddrInet(const struct sockaddr_in& addr)
            {
                addr_ = addr;
            }

            uint32_t ipNetEndian() const
            {
                return addr_.sin_addr.s_addr;
            }

            uint16_t portNetEndian() const
            {
                return addr_.sin_port;
            }

            std::string toIp() const;
            std::string toIpPort() const;
            uint16_t toPort() const;

        private:
            struct sockaddr_in addr_;

    };

}
}
#endif
