#ifndef __MinMaxHeap_HPP
#define __MinMaxHeap_HPP


#include <vector>
#include <stdexcept>
#include <iostream>	
#include <utility>	 
#include <algorithm>
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
	inline bool haveChild(const size_t& i) {
		return i*2 + 1 < v.size();
	}
	inline bool haveGrandchild(const size_t& i) {
		return i*4 + 3 < v.size();	
	}
/*
	inline Element<T>* min_element(const size_t& a, const size_t& b) {
		return &*std::min_element(v.begin() + a, v.begin() + b, 
								[](Element<T> a, Element<T> b)->bool{
									return a.key < b.key;
								}
							);
	}
	inline Element<T>* max_element(const size_t& a, const size_t& b) {
		return &*std::max_element(v.begin() + a, v.begin() + b,
								  [](Element<T> a, Element<T> b)->bool {
									  return a.key < b.key;
								  }
		);
	}

	inline Element<T>* minChild(const size_t& i) { //child musi istniec
		if (i + 2 < v.size())// wszystkie dzieci			
			return min_element(i + 1, i + 2);
		else
			return &v.back();	 // tylko jeden element
	}
	inline Element<T>* maxChild(const size_t& i) { //child musi istniec
		if (i + 1 < v.size())// wszystkie dzieci			
			return min_element(i + 1, i + 2);
		else
			return &v.back();	 // tylko jeden element
	}
	inline Element<T>* minGrandchild(const size_t& i) {// Granchild MUSI istniec
		if (i + 6 < v.size())// wszystkie wnuki
			return min_element(i + 3, i + 6);
		else
			return min_element(i, v.size() - 1);
	}
	inline Element<T>* maxGrandchild(const size_t& i) {// Granchild MUSI istniec
		if (i + 6 < v.size())// wszystkie wnuki
			return max_element(i + 3, i + 6);
		else
			return max_element(i, v.size() - 1);
	}
*/	
	inline size_t minChild(const size_t& i) { //child musi istniec
		auto lchild = LCHILD(i);
		if (lchild + 1 < v.size())// wszystkie dzieci			
			return v[lchild].key < v[lchild + 1].key ? lchild : lchild + 1;
		else
			return lchild;	 // tylko jeden element
	}
	inline size_t minGrandchild(const size_t& i) {// Granchild MUSI istniec
		auto minLchild = minChild(LCHILD(i));
		if (auto rChild = RCHILD(i); LCHILD(rChild) < v.size()) {// jezeli jest conajmniej 3 wnukow
			auto minRchild = minChild(rChild);
			return v[minLchild].key < v[minRchild].key ? minLchild : minRchild;
		}else
			return minLchild;

	}
	inline size_t maxChild(const size_t& i) { //child musi istniec
		auto lchild = LCHILD(i);
		if (lchild + 1 < v.size())// wszystkie dzieci			
			return v[lchild].key < v[lchild + 1].key ? lchild + 1 : lchild;
		else
			return lchild;	 // tylko jeden element
	}
	inline size_t maxGrandchild(const size_t& i) {// Granchild MUSI istniec
		auto maxLchild = maxChild(LCHILD(i));
		if (auto rChild = RCHILD(i); LCHILD(rChild) < v.size()) {// jezeli jest conajmniej 3 wnukow
			auto maxRchild = maxChild(rChild);
			return v[maxLchild].key < v[maxRchild].key ? maxRchild : maxLchild;
		}
		else
			return maxLchild;
	}
	inline unsigned int /*__declspec(naked)*/ elementLevel(size_t number) {// TODO asember ???
		unsigned int res = 0;
		while (haveParent(number)) {
			++res;
			number = PARENT(number);
		}
		return res;

/*		auto level;
		asm("bsrl %1, %0"
			: "=r" (level)
			: "r" (i));
		return level;
		*/

		//unsigned long result;
		//_BitScanForward(&result, number);
		//return result;

		//__asm {
		//	mov rax, number
		//	bsr rax, rax
		//	ret
		//}

		//int i;
		//unsigned int position = 0;
		//for (i = (number >> 1); i != 0; ++position)
		//	i >>= 1;

		//return position;
	}
	/*
	inline bool oddLevel(const size_t& i) { // TODO jakas idea??  NIE DZIALA PIERWSZYCH 2 POZIOMOW
		if (auto parent = PARENT(i); v[parent].key < v[PARENT(parent)].key)
			return false;
		else
			return true;
	}
	*/	 
	inline bool oddLevel(const size_t& i) { // TODO jakas idea??  NIE DZIALA PIERWSZYCH 2 POZIOMOW
		// return elementLevel(i + 1) % 2 == 0;
		return elementLevel(i) % 2 == 0;
	}

	inline bool isRChild(const size_t& i) {
		return !(i % 2);
	}

	inline void swap(const size_t& a, const size_t& b) {
		//std::cout << "zamieniam " << v[a].key << " z " << v[b].key << '\n';
		std::swap(v[a], v[b]);
	}
	inline void pushDownMin(size_t current) {
		while (true) {
			if (haveGrandchild(current)) {// ma wnukow wiec zamieniami z nimi jak trzeba po czym zamieniamy z dzieckiem
				auto minGc = minGrandchild(current);
				if (auto rchild = RCHILD(current); !haveChild(rchild) && v[rchild].key < v[current].key && v[rchild].key < v[minGc].key) {
					swap(rchild, current);
					return;
				}
				if (v[minGc].key < v[current].key) {
					swap(minGc, current);
					current = minGc;
					if (auto parent = PARENT(current); v[parent].key < v[current].key)
						swap(parent, current);
					continue;
				}
				return;
			}
			else if (haveChild(current)) { //nie ma wnukow, ale ma dzieci
				if (auto minC = minChild(current); v[minC].key < v[current].key)
					swap(minC, current);
				return; // juz nie ma z kim zamieniac
			}
			else return;// nie ma dzieci - nie ma z kim zamieniac
		}
	}
	inline void pushDownMax(size_t current) {// TODO from index to pointers??
		while (true) {
			if (haveGrandchild(current)) {// ma wnukow wiec zamieniami z nimi jak trzeba po czym zamieniamy z dzieckiem
				auto maxGc = maxGrandchild(current);
				if (auto rchild = RCHILD(current); !haveChild(rchild) && v[current].key < v[rchild].key && v[maxGc].key < v[rchild].key) {
					swap(rchild, current);
					return;
				}
				if (v[current].key < v[maxGc].key) {
					swap(maxGc, current);
					current = maxGc;
					if (auto parent = PARENT(current);  v[current].key < v[parent].key)
						swap(parent, current);
					continue;
				}
				return;
			}
			else if (haveChild(current)) { //nie ma wnukow, ale ma dzieci
				if (auto maxC = maxChild(current);  v[current].key < v[maxC].key)
					swap(maxC, current);
				return; // juz nie ma z kim zamieniac
			}
			else return;// nie ma dzieci - nie ma z kim zamieniac
		}
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
			else if (v.size() == 1)
				return v[0];
			else
				return v[1];
		}
		else if (v[1].key < v[2].key)
			return v[2];
		else
			return v[1];

	}
	void removeMin() {
		if (v.size() < 3) {	// przypadki indywidualne
			if (v.empty()) {
				throw std::exception("heap is empty");
			}
			else if (v.size() == 2) {// 2 elementy
				v.front() = v.back();
				v.pop_back();
			}
			else {// 1 element
				v.pop_back();
			}
		}else {
			v.front() = v.back();
			v.pop_back();	// teraz naprawa struktury, element pod indeksem 0 jest na zlym miejscu
			pushDownMin(0);
		}
	}
	void removeMax() {
		if (v.size() < 3) {	// przypadki indywidualne
			if (v.empty()) {
				throw std::exception("heap is empty");
			}
			else {// 1 element albo 2 elementy
				v.pop_back();
			}
		}
		else { // mamy conajmniej 3 elementy musiby wybrac max z 2
			if (v[1].key < v[2].key) {
				if (2 != v.size()) v[2] = v.back(); // jezeli nie jest ostanim elementem to zastepujemy ostatnim
				v.pop_back();	// teraz naprawa struktury, element pod indeksem 2 jest na zlym miejscu
				pushDownMax(2);
			}
			else {
				v[1] = v.back();
				v.pop_back();
				pushDownMax(1);
			}
		}
	}
	void put(const Element<T>& e) {
		auto current = v.size();
		v.emplace_back(e);

		if (!haveParent(current))
			return;
		else if (!haveGrandparent(current)) {
			if (v[current].key < v[0].key)
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

			auto s = v.size();

			while (true) {
				if (auto parent = PARENT(current); haveParent(current)) {
					if (levelType == (v[parent].key < currentKey)) {
						swap(current, parent);
						current = parent;
						levelType = !levelType;
						continue;
					}
					else if (auto grandpa = PARENT(parent); haveParent(parent)) {
						if (levelType == (currentKey < v[grandpa].key)) {
							swap(current, grandpa);
							current = grandpa;
							continue;
						}
					}
				}
				return;
			}
			/*
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
			*/
		}
	}
	void dumpKeys() {
		std::cout << '\n';
		std::cout << '\n';
		std::cout << '\n';
		std::cout << '\n';
		if(!v.empty())
			print(v);
	}
};

#endif