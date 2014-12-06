#ifndef _MYNET_NET_ENDIAN_
#define _MYNET_NET_ENDIAN_
#include <stdint.h>
#include <endian.h>
namespace MyNet
{
namespace net
{

namespace sockets
{
    inline uint64_t hostToNetwork64(uint64_t host64)
    {
        return htobe64(host64);
    }

    inline uint32_t hostToNetwork32(uint32_t host32)
    {
        return htobe32(host32);
    }

    inline uint16_t hostToNetwork16(uint16_t host16)
    {
        return htobe16(host16);
    }

    inline uint64_t networkToHost64(uint64_t network64)
    {
        return be64toh(network64);
    }

    inline uint32_t networkToHost32(uint32_t network32)
    {
        return be32toh(network32);
    }

    inline uint16_t networkToHost16(uint16_t network16)
    {
        return be16toh(network16);
    }
}

}
}
#endif
