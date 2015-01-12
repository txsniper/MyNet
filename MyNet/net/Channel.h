#ifndef _MYNET_NET_CHANNEL_H_
#define _MYNET_NET_CHANNEL_H_
#include "../base/NonCopyable.h"
#include "../base/Timestamp.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace MyNet
{
namespace net
{
    class EventLoop;

    class Channel : MyNet::Noncopyable
    {
        public:
            typedef boost::function< void () > EventCallback;
            typedef boost::function< void (Timestamp) > ReadEventCallback;

            Channel(EventLoop* loop, int fd);
            ~Channel();

            void handleEvent(Timestamp receivedTime);
            void setReadCallback(const ReadEventCallback& cb)
            {
                readCallback_ = cb;
            }
            void setWriteCallback(const EventCallback& cb)
            {
                 writeCallback_ = cb;
            }
            void setErrorCallback(const EventCallback& cb)
            {
                errorCallback_ = cb;
            }
            void setCloseCallback(const EventCallback& cb)
            {
                closeCallback_ = cb;
            }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
            void setReadCallback(ReadEventCallback&& cb)
            {
                readCallback_ =  std::move(cb);
            }
            void setWriteCallback(EventCallback&& cb)
            {
                writeCallback_ = std::move(cb);
            }
            void setCloseCallback(EventCallback&& cb)
            {
                closeCallback_ = std::move(cb);
            }
            void setErrorCallback(EventCallback&& cb)
            {
                errorCallback_ = std::move(cb);
            }
#endif
            ///Tie this channel to the owner object managed by shared_ptr,
            /// prevent the owner object being destroyed in handleEvent.
            void tie(const boost::shared_ptr<void>&);
            int fd() const { return fd_; }
            int events() const { return events_; }
            void set_revents(int revents) { revents_ = revents;}

            bool isNoneEvent() const
            {
                return events_ == kNoneEvent;
            }

            void enableReading() { events_ |= kReadEvent; update();}
            void disableReading() { events_ |= ~kReadEvent; update();}
            void enableWriting() { events_ |= kWriteEvent; update();}
            void disableWriting() { events_ |= ~kWriteEvent; update();}
            void disableAll() { events_ = kNoneEvent; update();}
            bool isWriting() const { return events_ & kWriteEvent; }

            int index() { return index_; }
            void set_index(int idx) { index_ = idx; }

            string reventsToString() const;
            void doNotLogHup() { logHup_ = false; }
            EventLoop* ownerLoop() const {return loop_;}
            void remove();
        private:

            void update();
            void handleEventWithGuard(Timestamp receiveTime);

            static const int kNoneEvent;
            static const int kReadEvent;
            static const int kWriteEvent;

            EventLoop* loop_;
            const int fd_;
            int index_;
            int events_;
            int revents_;
            bool logHup_;

            boost::weak_ptr<void> tie_;
            bool tied_;
            bool eventHandling_;
            bool addedToLoop_;
            ReadEventCallback readCallback_;
            EventCallback writeCallback_;
            EventCallback closeCallback_;
            EventCallback errorCallback_;

    };
}
}
#endif
