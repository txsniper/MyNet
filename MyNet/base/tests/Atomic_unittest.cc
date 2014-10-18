#include "../Atomic.h"
#include <assert.h>

int main()
{
    {
        MyNet::base::AtomicInt32 a32;
        assert(a32.get() == 0);
        assert(a32.getAndSet(1) == 0);
        assert(a32.addAndGet(1) == 2);
        assert(a32.getAndAdd(1) == 2);
        assert(a32.get() == 3);
        a32.inc();
        assert(a32.get() == 4);
        assert(a32.incAndGet() == 5);
        assert(a32.subAndGet(5) == 0);
        assert(a32.decAndGet() == -1);
        a32.dec();
        assert(a32.addAndGet(-1) == -3);
        assert(a32.subAndGet(-3) == 0);
    }
    {
        MyNet::base::AtomicInt64 a64;
        assert(a64.get() == 0);
        assert(a64.subAndGet(-3) == 3);
        assert(a64.addAndGet(-1) == 2);
        a64.dec();
        assert(a64.decAndGet() == 0);
        assert(a64.subAndGet(5) == -5);
        assert(a64.incAndGet() == -4);
        assert(a64.get() == -4);
        a64.inc();
        assert(a64.getAndSet(5) == -3);
        assert(a64.get() == 5);
    }
}

