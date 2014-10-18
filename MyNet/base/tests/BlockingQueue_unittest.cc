#include "../BlockingQueue.h"
#include "../Thread.h"
#include "../CountDownLatch.h"

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/bind.hpp>
#include <string.h>
#include <stdio.h>
class Test
{
    public:
        Test(int numThreads)
            : latch_(numThreads),
              threads_(numThreads)
        {
            for(int i = 0; i < numThreads; ++i)
            {
                char name[32] = {0};
                ::snprintf(name, sizeof(name), "worker thread %d", i);
                threads_.push_back(new MyNet::base::Thread(boost::bind(&Test::ThreadFunc, this), name));

            }
        }
    private:
        void ThreadFunc()
        {

        }

        MyNet::base::BlockingQueue<std::string> queue_;
        MyNet::base::CountDownLatch latch_;
        boost::ptr_vector<MyNet::base::Thread> threads_;
};


int main()
{

}
