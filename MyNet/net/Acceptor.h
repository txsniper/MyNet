#ifndef _MYNET_NET_ACCEPTOR_H
#define _MYNET_NET_ACCEPTOR_H

#include <boost/function.hpp>

#include "../base/NonCopyable.h"
#include "Channel.h"
#include "Socket.h"

namespace MyNet
{
namespace net
{

class EventLoop;
class InetAddress;

class Acceptor : MyNet::Noncopyable
{
 public:
  typedef boost::function<void (int sockfd,
                                const InetAddress&)> NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
  ~Acceptor();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }

  bool listenning() const { return listenning_; }
  void listen();

 private:

  // Acceptor处理读事件的函数
  void handleRead();

  // 事件循环
  EventLoop* loop_;

  // Acceptor 文件描述符
  Socket acceptSocket_;

  // Acceptor对应的Channel，用于管理事件处理函数
  Channel acceptChannel_;

  // 新连接到达时的处理函数
  NewConnectionCallback newConnectionCallback_;

  // 当前是否监听中
  bool listenning_;


  int idleFd_;
};

}
}

#endif  // MUDUO_NET_ACCEPTOR_H
