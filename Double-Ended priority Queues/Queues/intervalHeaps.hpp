#ifndef __SYMMETRICMINMAXHEAP_HPP
#define __SYMMETRICMINMAXHEAP_HPP


#include <vector>
#include <stdexcept>
#include <iostream>	
#include "QueueBase.hpp"


template<typename T>
class IntervalHeap : public QueueBase<T> {
	std::vector<Element<T>> v;

	inline int PARENT(const int& i) {
		return 	((i - 1) / 2);
	}
	inline int LCHILD(const int& i) {
		return 	2 * i + 1;
	}
	inline int RBROTHER(const int& i) {
		return 	i + 1;
	}
	inline int LBROTHER(const int& i) {
		return 	i - 1;
	}
	inline int RCHILD(const int& i) {
		return 	2 * i + 2;
	}
	inline bool haveGrandparent(const int& i) {
		return i > 2;
	}
	inline bool isRChild(const int& i) {
		return !(i % 2);
	}

	inline void swap(const size_t& a, const size_t& b) {
		//std::cout << "zamieniam " << v[a].key << " z " << v[b].key << '\n';
		std::swap(v[a], v[b]);
	}
public:
	IntervalHeap() {
		v.emplace_back();
	}
	~IntervalHeap() {}

};

#endif