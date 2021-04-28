#ifndef __QUEUEBASE_HPP
#define __QUEUEBASE_HPP

#include "../Element.hpp"  
#include "../PrintTree.hpp"


template<typename T>
class QueueBase {
	public:
	virtual Element<T> getMin() = 0;
	virtual Element<T> getMax() = 0;
	virtual void removeMin() = 0;
	virtual void removeMax() = 0;
	virtual void put(Element<T>&) = 0;
	virtual void dumpKeys() = 0;
};

#endif
