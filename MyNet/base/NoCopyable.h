#ifndef _BASE_NO_COPYABLE_H_
#define _BASE_NO_COPYABLE_H_

namespace MyNet
{

class Nocopyable
{
protected:
	Nocopyable() {}
	~Nocopyable() {}
private:
	Nocopyable(const Nocopyable& lhs);
	Nocopyable& operator = (const Nocopyable& lhs);

};
}

#endif