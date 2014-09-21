#ifndef _BASE_NO_COPYABLE_H_
#define _BASE_NO_COPYABLE_H_

namespace MyNet
{

class Noncopyable
{
protected:
	Noncopyable() {}
	~Noncopyable() {}
private:
	Noncopyable(const Noncopyable& lhs);
	Noncopyable& operator = (const Noncopyable& lhs);

};
}

#endif
