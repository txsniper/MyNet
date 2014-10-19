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
            for_each(threads_.begin(), threads_.end(), boost::bind(&MyNet::base::Thread::start, _1) );
        }

        void run(int times)
        {
            ::printf("waiting for all threads started\n");
            latch_.wait();
            ::printf("all threads started\n");
            for(int i=0; i<times; ++i)
            {
                char buf[32];
                ::snprintf(buf, sizeof(buf), "hello %d", i);
                queue_.put(buf);
                printf("tid=%d, put data = %s, size = %zd\n", MyNet::ThreadOp::tid(), buf, queue_.size());
            }
        }

        void joinAll()
        {
            for(size_t i=0; i<threads_.size(); ++i)
            {
                queue_.put("STOP");
            }
            for_each(threads_.begin(), threads_.end(), boost::bind(&MyNet::base::Thread::join, _1));
        }
    private:
        void ThreadFunc()
        {
            ::printf("tid=%d, %s started\n", MyNet::ThreadOp::tid(), MyNet::ThreadOp::threadName());
            latch_.countDown();
            bool running = true;
            while(running)
            {
                std::string tmp(queue_.get());
                printf("tid=%d get data=[%s] size=%zd\n", MyNet::ThreadOp::tid(),
                        tmp.c_str(), queue_.size());
                running = (tmp != "STOP");
            }
            ::printf("tid=%d %s stoped\n", MyNet::ThreadOp::tid(),
                    MyNet::ThreadOp::threadName());
        }
        MyNet::base::BlockingQueue<std::string> queue_;
        MyNet::base::CountDownLatch latch_;
        boost::ptr_vector<MyNet::base::Thread> threads_;
};


int main()
{
    ::printf("Main thread tid=%d pid=%d\n", MyNet::ThreadOp::tid(),
            ::getpid());
    Test t(5);
    t.run(50);
    t.joinAll();
    ::printf("number of created threads: %d\n", MyNet::base::Thread::threadNum());
    return 0;
}
