#include "InetAddress.h"
#include "SocketsOps.h"
#include "Endian.h"

#include <string.h>

#pragma GCC diagnostic ignored "-Wold-style-cast"
static const in_addr_t kInAddrAny = INADDR_ANY;
static const in_addr_t kInAddrLoopback = INADDR_LOOPBACK;
#pragma GCC diagnostic error "-Wold-style-cast"

namespace MyNet
{
namespace net
{

    InetAddress::InetAddress(const std::string& ip_addr, uint16_t port)
    {
        ::bzero(&addr_, sizeof addr_);
        sockets::fromIpPort(ip_addr.c_str(), port, &addr_);
    }

    InetAddress::InetAddress(uint16_t port, bool loopbackonly)
    {
        ::bzero(&addr_, sizeof addr_);
        addr_.sin_family = AF_INET;
        addr_.sin_port = sockets::hostToNetwork16(port);
        if (loopbackonly)
            addr_.sin_addr.s_addr = sockets::hostToNetwork32(kInAddrLoopback);
        else
            addr_.sin_addr.s_addr = sockets::hostToNetwork32(kInAddrAny);
    }

    std::string InetAddress::toIp() const
    {
        char buf[32] = {0};
        sockets::toIp(buf, sizeof buf, addr_);
        return buf;
    }

    std::string InetAddress::toIpPort() const
    {
        char buf[32] = {0};
        sockets::toIpPort(buf, sizeof buf, addr_);
        return buf;
    }

    uint16_t InetAddress::toPort() const
    {
        return sockets::networkToHost16(addr_.sin_port);
    }


}
}
