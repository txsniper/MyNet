#include "Buffer.h"
#include "SocketsOps.h"

#include <errno.h>
#include <sys/uio.h>

using namespace MyNet;
using namespace MyNet::net;

const char Buffer::kCRLF[] = "\r\n";
const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;


