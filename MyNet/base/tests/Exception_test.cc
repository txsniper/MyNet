#include "../Exception.h"
#include <stdio.h>
using namespace MyNet::base;

class ATest
{
public:
    void test()
    {
        throw Exception("oops");
    }
};

void call()
{
    ATest a;
    a.test();
}

int main()
{
    try
    {
        call();
    }
    catch(const Exception& e)
    {
        printf("exception message : %s\n", e.what());
        printf("exception stack: %s\n", e.stackTrace());
    }
    return 0;
}
