#ifndef __MinMaxHeap_HPP
#define __MinMaxHeap_HPP


#include <vector>
#include <stdexcept>
#include <iostream>	
#include <utility>
#include "QueueBase.hpp"


template<typename T>
class MinMaxHeap : public QueueBase<T> {
	std::vector< Element<T> > v;

	inline size_t PARENT(const size_t& i) {
		return 	((i - 1) / 2);
	}
	inline size_t LCHILD(const size_t& i) {
		return 	2 * i + 1;
	}
	inline size_t RCHILD(const size_t& i) {
		return 	2 * i + 2;
	}
	inline size_t RBROTHER(const size_t& i) {
		return 	i + 1;
	}
	inline size_t LBROTHER(const size_t& i) {
		return 	i - 1;
	}
	inline bool haveGrandparent(const size_t& i) {
		return i > 2;
	}
	inline bool haveParent(const size_t& i) {
		return i > 0;
	}

	inline bool oddLevel(const size_t& i) { // TODO jakas idea??
		if (auto parent = PARENT(i); v[parent].key < v[PARENT(parent)].key)
			return false;
		else
			return true;
	}
	inline bool isRChild(const size_t& i) {
		return !(i % 2);
	}

	inline void swap(const size_t& a, const size_t& b) {
		//std::cout << "zamieniam " << v[a].key << " z " << v[b].key << '\n';
		std::swap(v[a], v[b]);
	}

public:
	MinMaxHeap() {}
	~MinMaxHeap() {}

	Element<T> getMin() {
		if (v.empty())
			throw std::exception("heap is empty");
		else
			return v.front();
	}
	Element<T> getMax() {
		if (v.size() < 3) {
			if (v.empty())
				throw std::exception("heap is empty");
			else if (v.size() == 2)
				return v[1];
		}
		else if (v[1].key < v[2].key)
			return v[2];
		else
			return v[1];

	}
	void removeMin() {
	}
	void removeMax() {
	}
	void put(Element<T>& e) {
		auto current = v.size();
		v.emplace_back(e);

		if (!haveParent(current))
			return;
		else if (!haveGrandparent(current)) {
			if (v.front().key > v[current].key)
				swap(0, current);
		}
		else { // przypadki 2 pierwszych poziomow rozpatrzone
			bool levelType = oddLevel(current);
			const auto currentKey = v[current].key;
			auto parent = PARENT(current);
			auto grandparent = PARENT(parent);

			if (levelType) {// level min
				if (v[parent].key < currentKey) {
					swap(current, parent);
					current = parent;
					levelType = !levelType;
				}
				else if (currentKey < v[grandparent].key) {
					swap(current, grandparent);
					current = grandparent;
				}
				else return; // wszystko ok
			}
			else {//level max 
				if (currentKey < v[parent].key) {
					swap(current, parent);
					current = parent;
					levelType = !levelType;
				}
				else if (v[grandparent].key < currentKey) {
					swap(current, grandparent);
					current = grandparent;
				}
				else return; // wszystko ok
			} 
	
			if (levelType) {	// level min
				while (haveGrandparent(current)) {
					grandparent = PARENT(PARENT(current));
					if (currentKey < v[grandparent].key) {
						swap(current, grandparent);
						current = grandparent;
					}
					else return;
				}
			}
			else {	// level max
				while (haveGrandparent(current)) {
					grandparent = PARENT(PARENT(current));
					if (v[grandparent].key < currentKey) {
						swap(current, grandparent);
						current = grandparent;
					}
					else return;
				}
			}
		}			  
	}
	void dumpKeys() {
		std::cout << '\n';
		std::cout << '\n';
		std::cout << '\n';
		std::cout << '\n';
		print(v);
	}
};

#endif