#ifndef __SYMMETRICMINMAXHEAP_HPP
#define __SYMMETRICMINMAXHEAP_HPP


#include <vector>
#include <stdexcept>
#include <iostream>	
#include <cassert>
#include "QueueBase.hpp"


template<typename T>
class SymmetricMinMaxHeap : public QueueBase<T> {
	std::vector<Element<T>> v;

	inline size_t PARENT(const size_t& i) {
		return 	((i - 1) / 2);
	}
	inline size_t LCHILD(const size_t& i) {
		return 	2 * i + 1;
	}	
	inline size_t RBROTHER(const size_t& i) {
		return 	i+1;
	}
	inline size_t LBROTHER(const size_t& i) {
		return 	i-1;
	}
	inline size_t RCHILD(const size_t& i) {
		return 	2 * i + 2;
	}
	inline bool haveGrandparent(const size_t& i) {
		return i > 2;
	}
	inline bool isRChild(const size_t& i) {
		return !(i % 2);
	}

	inline void swap(const size_t& a,const size_t& b) {
		//std::cout << "zamieniam " << v[a].key << " z " << v[b].key << '\n';
		std::swap(v[a], v[b]);
	}

	void keyDumpHelper(std::string prefix, std::size_t n, bool isLeft) {
		if (n < v.size()) {
			std::cout << prefix << "|-- " << v[n].key << '\n';
			keyDumpHelper(prefix + (isLeft ? "|   " : "    "), LCHILD(n), true);
			keyDumpHelper(prefix + (isLeft ? "|   " : "    "), RCHILD(n), false);
		}
	}
	void keyDumpHelper2(std::string prefix, std::size_t level) {
		int range = 1;
		for (int j = 1; j <= level; j++)range *= 2;
		
		for (int i = range; i < 2 * range; i++) {
			if (i >= v.size())return;
			std::cout << prefix << v[i].key << " ";
		}
		std::cout << '\n';
		prefix.resize(prefix.size()/2);
		keyDumpHelper2(prefix, ++level);
	}
	void keyDumpHelper3() {
		print(v);
	}

	void tickleDown(size_t i) {
		const auto size = v.size();
		while (true) {
			//dumpKeys();
			const auto& currentKey = v[i].key;
			if (isRChild(i)) {  // jest prawym dzieckiem
				if (v[i - 1].key > v[i].key) {// czy zamienic z bratem
					swap(i, i-1); i--;
					continue;
				}
				
				auto LBrotherRChild = RCHILD(i - 1);
				if (auto RChild = RCHILD(i); RChild < size && currentKey < v[RChild].key && v[LBrotherRChild].key < v[RChild].key) {
					swap(RChild, i);
					i = RChild;
					continue;
				}
				else if ( LBrotherRChild < size && currentKey < v[LBrotherRChild].key) {
					swap(LBrotherRChild, i);
					i = LBrotherRChild;
					continue;
				}
				else break; // struktura nie wymaga juz poprawy
			}
			else if (i + 1 < v.size()) { // jest lewym dzieckiem && ma prawego brata
				if (v[i].key > v[i + 1].key) {// czy zamienic z bratem
					swap(i, i + 1);i++;
					continue;
				}
				auto LChild = LCHILD(i);
				if (auto RBrotherLChild = LCHILD(i + 1); RBrotherLChild < size && v[RBrotherLChild].key < currentKey && v[RBrotherLChild].key < v[LChild].key) {
					swap(RBrotherLChild, i);
					i = RBrotherLChild;
					continue;
				}
				else if ( LChild < size && v[LChild].key < currentKey) {
					swap(LChild, i);
					i = LChild;
					continue;
				}
				else break; // struktura nie wymaga juz poprawy
			}									
			else break;// jest lewym dzieckiem && nie ma prawego brata
		}													
	}

	inline void pushUp(size_t newElementCurrentIndex) {
		if (0 && haveGrandparent(newElementCurrentIndex)) {
			auto grandparentIndex = PARENT(PARENT(newElementCurrentIndex));
			auto [elementKey, _] = v[newElementCurrentIndex];
			if (auto temp = LCHILD(grandparentIndex); elementKey < v[temp].key) {
				do{ 
					swap(temp, newElementCurrentIndex);
					newElementCurrentIndex = temp;
					elementKey = v[newElementCurrentIndex].key;
					grandparentIndex = PARENT(PARENT(newElementCurrentIndex));
					temp = LCHILD(grandparentIndex);
				} while (haveGrandparent(newElementCurrentIndex) && elementKey < v[temp].key);

			}
			else if (auto temp = RCHILD(grandparentIndex); v[temp].key < elementKey) { // temp++ dla optymalnosci? -przejrzystosc
				do {
					swap(temp, newElementCurrentIndex);
					newElementCurrentIndex = temp;
					elementKey = v[newElementCurrentIndex].key;
					grandparentIndex = PARENT(PARENT(newElementCurrentIndex));
					temp = RCHILD(grandparentIndex);
				} while (haveGrandparent(newElementCurrentIndex) && v[temp].key < elementKey);
			}
			else return;
		}
		while (haveGrandparent(newElementCurrentIndex)) {
			auto grandparentIndex = PARENT(PARENT(newElementCurrentIndex));
			auto [elementKey, _] = v[newElementCurrentIndex];

			if (auto temp = LCHILD(grandparentIndex); elementKey < v[temp].key) {
				swap(temp, newElementCurrentIndex);
				newElementCurrentIndex = temp;

			}
			else if (auto temp = RCHILD(grandparentIndex); v[temp].key < elementKey) { // temp++ dla optymalnosci? -przejrzystosc
				swap(temp, newElementCurrentIndex);
				newElementCurrentIndex = temp;
			}
			else return;
		};
	}
	inline void fixWithBrother(size_t elementIndex) {
		if (isRChild(elementIndex)) {
			if (auto l = LBROTHER(elementIndex);  v[elementIndex].key < v[l].key) {
				swap(elementIndex, l);
				elementIndex = l;
			}
		}
		else if (auto r = RBROTHER(elementIndex); r < v.size() && v[r].key < v[elementIndex].key) {
			swap(elementIndex, r);
			elementIndex = r;
		}
	}
	
	inline [[nodiscard]] size_t pushMinDown() { // contexpr z max dla przejrzystosci?
		size_t elementCurrentIndex = 1;
		while (true) {
			if (auto left = LCHILD(elementCurrentIndex); left < v.size()) {
				const auto& leftKey = v[left].key;
				if (auto right = LCHILD(RBROTHER(elementCurrentIndex)); right < v.size())
					if (const auto& rightKey = v[right].key; rightKey <= leftKey) {
						v[elementCurrentIndex] = v[right];
						elementCurrentIndex = right;
						continue;
					}
				v[elementCurrentIndex] = v[left];
				elementCurrentIndex = left;
			}else break;
		}
		return elementCurrentIndex;
	}

	inline [[nodiscard]] size_t pushMaxDown() {
		size_t elementCurrentIndex = 2;
		while (true) {
			if (auto left = RCHILD(LBROTHER(elementCurrentIndex)); left < v.size()) {
				const auto& leftKey = v[left].key;
				if (auto right = RCHILD(elementCurrentIndex); right < v.size())
					if (const auto& rightKey = v[right].key;  leftKey <= rightKey) {
						v[elementCurrentIndex] = v[right];
						elementCurrentIndex = right;
						continue;
					}
				v[elementCurrentIndex] = v[left];
				elementCurrentIndex = left;
			}
			else break;
		}
		return elementCurrentIndex;
	}

	public:
	SymmetricMinMaxHeap() {
		v.emplace_back();
	}
	~SymmetricMinMaxHeap(){}

	Element<T> getMin() { 
		if (v.size() == 1)
			throw std::exception("heap is mpty");
		else
			return v[1]; 
	}
	Element<T> getMax() { 
		if (v.size() == 1)
			throw std::exception("heap is empty");
		else if(v.size() == 2)
			return v[1];
		else 
			return v[2]; 
	}

	void removeMinAndAdd(size_t size, size_t root, Element<T>& res, Element<T>& x) {
		const auto lchild = LCHILD(root), rchild = RCHILD(root);

		if (size <= lchild || x.key < v[lchild].key) {
			res = x;
			return;
		}

		res = v[lchild];

		if (rchild < size && v[rchild].key < x.key)
			std::swap(v[rchild], x);

		if (LCHILD(rchild) < size && v[LCHILD(rchild)].key < v[LCHILD(lchild)].key)
			removeMinAndAdd(size, rchild, v[lchild], x);
		else if (LCHILD(lchild) < size)
			removeMinAndAdd(size, lchild, v[lchild], x);
		else
			v[lchild] = x;
		
	}

	void removeMaxAndAdd(size_t size, size_t root, Element<T>& res, Element<T>& x) {
		const auto lchild = LCHILD(root), rchild = RCHILD(root);


		if (rchild < size) { // 2 dzieci
			if (v[rchild].key < x.key)
				res = x;
			else {
				res = v[rchild];

				if (x.key < v[lchild].key)
					std::swap(x, v[lchild]);

				if (RCHILD(rchild) < size && v[RCHILD(lchild)].key < v[RCHILD(rchild)].key)
					removeMaxAndAdd(size, rchild, v[rchild], x);
				else if (RCHILD(lchild) < size)
					removeMaxAndAdd(size, lchild, v[rchild], x);
				else
					v[rchild] = x;
			}
		}
		else if (rchild == size) { // 1 dziecko
			if (v[lchild].key < x.key)
				res = x;
			else {
				res = v[lchild];
				v[lchild] = x;
			}
		}
		else { // 0 dzieci
			res = v[rchild];
			v[rchild] = x;
		}
	}

	void removeMin() {
		//assert(!v.empty());
		auto x = v.back(); v.pop_back();
		if (v.size() != 1) {
			Element<T> res;
			removeMinAndAdd(v.size(), 0, res, x);
		}
	}

	void _removeMin() {
		auto size = v.size();
		if (size <= 3) { // przypadki indywidualne
			if (size == 3) {
				v[1] = v[2];
				v.pop_back();
			}
			else if (size == 2) {
				v.pop_back();
			}
			else throw std::exception("heap is empty");

			return;
		}

		// drzewo ma conajmniej jeden element na glebokosci 2
		/*
			v[1] = v[size - 1];
			v.pop_back();
			// usunelismy element a w miejsce min jest nowy z konca, trzeba naprawic strukture
			tickleDown(1);
		*/
		auto elementIndex = pushMinDown();
		if (elementIndex < size - 1) {
			v[elementIndex] = v.back();
			fixWithBrother(elementIndex);
			pushUp(elementIndex);
		}
		v.pop_back();
	}

	void removeMax() {
		//assert(!v.empty());
		auto x = v.back(); v.pop_back();
		if (2 < v.size()) {
			Element<T> res;
			removeMaxAndAdd(v.size(), 0, res, x);
		}
	}

	void _removeMax() {
		auto size = v.size();
		if (size <= 3) { // przypadki indywidualne
			if (size == 3) {
				v.pop_back();
			}
			else if (size == 2) {
				v.pop_back();
			}
			else throw std::exception("heap is empty");
			return;
		}
		// drzewo ma conajmniej jeden element na glebokosci 2
		/*
			v[2] = v[size - 1];
			v.pop_back();
			// usunelismy element a w miejsce min jest nowy z konca, trzeba naprawic strukture
			tickleDown(2);
		 */
		auto elementIndex = pushMaxDown();
		if (elementIndex < size - 1) {
			v[elementIndex] = v.back();
			fixWithBrother(elementIndex);
			pushUp(elementIndex);
		}
		v.pop_back();


	}
	void put(const Element<T>& e) {
		
		if (auto size = v.size(); size <= 2) { //  indywidualne przypadki gdy elementy nie maja dziadkow
			if (size == 2 && v[1].key > e.key)
				v.insert(v.begin() + 1, e);
			else
				v.emplace_back(e);

			return;
		}
		// wiemy ze conajmniej 3 elementy sa w strukturze
		auto newElementCurrentIndex = v.size();
		v.emplace_back(e);
		auto [elementKey, _] = v[newElementCurrentIndex];

		if (isRChild(newElementCurrentIndex) && elementKey < v[LBROTHER(newElementCurrentIndex)].key) {	// bezkarna zamiana z bratem-lisciem zachowa strukture
			swap(LBROTHER(newElementCurrentIndex), newElementCurrentIndex);
			--newElementCurrentIndex;
		}
		pushUp(newElementCurrentIndex);	   // mozna oszczedzic jedno sprawdzenie z rodzicem i jego bratem-rodzica, bo znamy nasza relacje z bratem, TODO? 
	}

	void dumpKeys() {
		std::cout << '\n';
		//keyDumpHelper("", 0, false);
		//keyDumpHelper2("                                                                                              ", 0);
		keyDumpHelper3();
	}
};

#endif