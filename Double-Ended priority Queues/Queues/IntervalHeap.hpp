#ifndef __IntervalHeap_HPP
#define __IntervalHeap_HPP


#include <vector>
#include <stdexcept>
#include <iostream>	
#include <utility>
#include "QueueBase.hpp"


template<typename T>
class IntervalHeap : public QueueBase<T> {
	std::vector< Element<T> > v;

	inline size_t PARENT(const size_t& i) {
		return 	2 * (((i / 2) - 1) / 2);
	}
	inline size_t LCHILD(const size_t& i) {
		return 	(2 * (i / 2) + 1) * 2;
	}
	inline size_t RCHILD(const size_t& i) {
		return 	(2 * (i / 2) + 2) * 2;
	}
	inline size_t RELEMENT(const size_t& i) {
		return 	i + 1;
	}
	inline size_t haveParent(const size_t& i) {
		return i > 1;
	}

	inline void swap(const size_t& a, const size_t& b) {
		//std::cout << "zamieniam " << v[a].key << " z " << v[b].key << '\n';
		std::swap(v[a], v[b]);
	}

	void keyDumpHelper(std::string prefix, std::size_t n, bool isLeft) {
		if (n < v.size()) {
			if(n + 1 < v.size())
				std::cout << prefix << "|-- " << v[n].key <<","<< v[n+1].key << '\n';
			else
				std::cout << prefix << "|-- " << v[n].key << '\n';

			keyDumpHelper(prefix + (isLeft ? "|   " : "    "), LCHILD(n), true);
			keyDumpHelper(prefix + (isLeft ? "|   " : "    "), RCHILD(n), false);
		}
	}

	void pushUp(size_t newElementCurrentIndex) {
		while (haveParent(newElementCurrentIndex)) {
			auto &[elementKey, _] = v[newElementCurrentIndex];
			auto parent = PARENT(newElementCurrentIndex);

			if (elementKey < v[parent].key) {
				swap(newElementCurrentIndex, parent);
				newElementCurrentIndex = parent;
			}
			else if (v[++parent].key < elementKey) {
				swap(newElementCurrentIndex, parent);
				newElementCurrentIndex = parent;
			}
			else return;
		}
	}
	void pushMinDown() { 
		size_t elementCurrentIndex = 0;
		for (auto lChild = LCHILD(elementCurrentIndex); lChild < v.size(); lChild = LCHILD(elementCurrentIndex)) {// poki istnieje lewe dziecko
			auto& cKey = v[elementCurrentIndex].key;
			auto& lChildKey = v[lChild].key;
			
			if (auto rChild = RCHILD(elementCurrentIndex); rChild < v.size()){
				if (auto& rChildKey = v[rChild].key; rChildKey < lChildKey && rChildKey < cKey) {	//jezeli mamy zamienic z prawa wartoscia
					swap(elementCurrentIndex, rChild);
					elementCurrentIndex = rChild;
				}else if (lChildKey < cKey) {
					swap(elementCurrentIndex, lChild);
					elementCurrentIndex = lChild;
				}
				else break;
			}
			else {
				if (lChildKey < cKey) {	   //jezeli mamy zamienic z lewa wartoscia
					swap(elementCurrentIndex, lChild);
					elementCurrentIndex = lChild;
				}else break;
			}

			if (auto rValue = RELEMENT(elementCurrentIndex); rValue < v.size() && v[rValue].key < v[elementCurrentIndex].key) { // zamiana z drugim elementem w nodzie, nie zmieniamy wtedy indexu
				swap(elementCurrentIndex, rValue);
			}
		}
	}
	void pushMaxDown() {
		size_t elementCurrentIndex = 1;
		for (auto lChild = RELEMENT(LCHILD(elementCurrentIndex)); lChild < v.size(); lChild = RELEMENT(LCHILD(elementCurrentIndex))) {// poki istnieje prawy element lewego dziecka
			auto& cKey = v[elementCurrentIndex].key;
			auto& lChildKey = v[lChild].key;

			if (auto rChild = RELEMENT(RCHILD(elementCurrentIndex)); rChild < v.size()) {
				if (auto& rChildKey = v[rChild].key; lChildKey < rChildKey && cKey < rChildKey) {	//jezeli mamy zamienic z prawa wartoscia
					swap(elementCurrentIndex, rChild);
					elementCurrentIndex = rChild;
				}
				else if (cKey < lChildKey ) {
					swap(elementCurrentIndex, lChild);
					elementCurrentIndex = lChild;
				}
				else break;
			}
			else {
				if (cKey < lChildKey) {	   //jezeli mamy zamienic z lewa wartoscia
					swap(elementCurrentIndex, lChild);
					elementCurrentIndex = lChild;
				}
				else break;
			}

			if (auto lValue = elementCurrentIndex - 1 ; v[elementCurrentIndex].key < v[lValue].key) { // zamiana z drugim elementem w nodzie, nie zmieniamy wtedy indexu
				swap(elementCurrentIndex, lValue);
			}
		}
	}

public:
	IntervalHeap() {}
	~IntervalHeap() {}

	Element<T> getMin() {
		if (v.empty())
			throw std::exception("heap is empty");
		else 
			return v.front();
		
	}
	Element<T> getMax() {
		if (v.size() < 2) { 
			if (v.empty())
				throw std::exception("heap is empty");
			else
				return v.front();
		}
		else
			return v[1];
	}
	void removeMin() {
		if (v.size() < 2) {
			if (v.empty())
				throw std::exception("heap is empty");

			v.pop_back();
			return;
		}
		v.front() = v.back();
		v.pop_back();
		pushMinDown(); //naprawa struktury
	}
	void removeMax() {
		if (v.size() < 3) {
			if (v.empty())
				throw std::exception("heap is empty");
			v.pop_back();
			return;
		}
		v[1] = v.back();
		v.pop_back();
		pushMaxDown(); //naprawa struktury

	}
	void put(Element<T>& e) {
		auto last = v.size();
		v.emplace_back(e);
		pushUp(last);
		
		if ((last & 1) && v[last].key < v[last - 1].key) // jezeli parzysta ilosc elementow to musimy zamienic z drugim elementem
			swap(last, last - 1);
	}
	void dumpKeys() {
		std::cout << "elementow jest : " << v.size()<<'\n';
		//for (auto e : v)
		//	std::cout << e.key <<" ";
		keyDumpHelper("", 0, false);
	}
};

#endif