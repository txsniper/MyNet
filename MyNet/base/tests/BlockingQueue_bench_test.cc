#include "../BlockingQueue.h"
#include "../Thread.h"
#include "../CountDownLatch.h"
#include "../Timestamp.h"

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/bind.hpp>
#include <string.h>
#include <map>
#include <stdio.h>

class Bench
{
    public:
        Bench(int numThreads)
            : latch_(numThreads),
              threads_(numThreads)
        {
            for(int i=0; i<numThreads; ++i)
            {
                char name[32] = {0};
                ::snprintf(name, sizeof(name), "worker thread %d", i);
                threads_.push_back(new MyNet::base::Thread(boost::bind(&Bench::ThreadFunc, this), name));
            }
            for_each(threads_.begin(), threads_.end(), boost::bind(&MyNet::base::Thread::start, _1));
        }

        void run(int times)
        {
            ::printf("waitting for all threads started\n");
            latch_.wait();
            ::printf("all threads started\n");
            for(int i=0; i<times; ++i)
            {
                MyNet::base::Timestamp now(MyNet::base::Timestamp::now());
                queue_.put(now);
                ::usleep(1000);
            }
        }

        void joinAll()
        {
            for(size_t i =0; i<threads_.size(); ++i)
            {
                queue_.put(MyNet::base::Timestamp::invalid());
            }
            for_each(threads_.begin(), threads_.end(), boost::bind(&MyNet::base::Thread::join, _1));
        }
    private:

        void ThreadFunc()
        {
            ::printf("tid=%d, %s started\n", MyNet::ThreadOp::tid(), MyNet::ThreadOp::threadName());
            std::map<int,int> delays;
            latch_.countDown();
            bool running = true;
            while(running)
            {
                MyNet::base::Timestamp t(queue_.get());
                MyNet::base::Timestamp now(MyNet::base::Timestamp::now());
                if(t.valid())
                {
                    int delay = static_cast<int>(MyNet::base::timeDifference(now, t) * 1000000);
                    ++delays[delay];
                }
                running = t.valid();
            }
            ::printf("tid=%d, %s STOPED\n", MyNet::ThreadOp::tid(),
                    MyNet::ThreadOp::threadName());
            for(std::map<int, int>::iterator it = delays.begin();
                    it != delays.end(); ++it)
            {
                ::printf("tid=%d, delay=%d, count=%d\n",
                        MyNet::ThreadOp::tid(), it->first, it->second);
            }
        }

        MyNet::base::CountDownLatch latch_;
        boost::ptr_vector<MyNet::base::Thread> threads_;
        MyNet::base::BlockingQueue<MyNet::base::Timestamp> queue_;
};

int main(int argc, char* argv[])
{
    int threads = argc > 1 ? atoi(argv[1]) : 1;
    Bench b(threads);
    b.run(10000);
    b.joinAll();
}
