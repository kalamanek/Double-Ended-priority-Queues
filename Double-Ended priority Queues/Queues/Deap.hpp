#ifndef __DEAP_HPP
#define __DEAP_HPP


#include <vector>
#include <stdexcept>
#include <iostream>	
#include "QueueBase.hpp"


template<typename T>
class Deap : public QueueBase<T> {
	std::vector<Element<T>> v;

	inline size_t PARENT(const size_t& i) {
		return 	((i - 1) / 2);
	}
	inline size_t LCHILD(const size_t& i) {
		return 	2 * i + 1;
	}
	inline size_t RCHILD(const size_t& i) {
		return 	2 * i + 2;
	}
	inline bool haveParent(const size_t& i) {
		return i > 2;
	}
	inline size_t minChild(const size_t& i) { //lchild musi istniec
		auto lchild = LCHILD(i);
		if (lchild + 1 < v.size())// wszystkie dzieci			
			return v[lchild].key < v[lchild + 1].key ? lchild : lchild + 1;
		else
			return lchild;	 // tylko jeden element
	}
	inline size_t maxChild(const size_t& i) { //lchild musi istniec
		auto lchild = LCHILD(i);
		if (lchild + 1 < v.size())// wszystkie dzieci			
			return v[lchild + 1].key < v[lchild].key ? lchild : lchild + 1;
		else
			return lchild;	 // tylko jeden element
	}
	inline unsigned int elementLevel(const size_t& number) {// TODO asember ???
/*		auto level;
		asm("bsrl %1, %0"
			: "=r" (level)
			: "r" (i));
		return level;
		*/
		int i;
		unsigned int position = 0;
		for (i = (number >> 1); i != 0; ++position)
			i >>= 1;

		return position;
	}

	inline std::pair<bool, size_t> isMinLevelAndCorresponding(const size_t& i) {		// diala tylko do inisertu (ostatni element w drzewie zupelnym)
		const auto level = elementLevel(i + 1);
		const int halfLevelIndexAmount = std::pow(2, level-1);
		const int LevelIndexAmount = halfLevelIndexAmount * 2;
		const auto isMinLevel = (i < LevelIndexAmount + ((LevelIndexAmount - 1) / 2) ? true : false);
		
		if (isMinLevel) {//minlvl
			auto optional = i + halfLevelIndexAmount;
			if (optional < v.size()) {// jezeli istnieje odpowiadajacy pozycji w max drzewku
				return std::make_pair(true, optional);
			}
			else {
				//auto cor = ((i + 1) / 2) - 2 + (LevelIndexAmount / 2); // nie istnieje, wiec jest to ojciec odpowiadajacego noda w drzewku max
				auto cor = (optional - 1) / 2; // jezeli nie istnieje to jego rodzic
				return std::make_pair(true, cor);
			}
		}
		else {//maxlvl poniewaz drzewo jest zupelnie zawsze istnieje odpowiadajacy node
			auto cor = i - halfLevelIndexAmount;
			return std::make_pair(false, cor);
		}
	}

	inline void swap(const size_t& a, const size_t& b) {
		//std::cout << "zamieniam " << v[a].key << " z " << v[b].key << '\n';
		std::swap(v[a], v[b]);
	}
	/*
	inline size_t pushMaxDown(size_t curr) {
		auto leftChild = LCHILD(curr);
		while (leftChild < v.size()) {
			auto min = maxChild(curr);
			if (v[curr].key < v[min].key) {
				swap(min, curr);
				curr = min;
			}else break;
			leftChild = LCHILD(curr);
		}
		return curr;
	}
	inline size_t pushMinDown(size_t curr) {
		auto leftChild = LCHILD(curr);
		while (leftChild < v.size()) {
			auto max = minChild(curr);
			if (v[max].key < v[curr].key) {
				swap(max, curr);
				curr = max;
			}else break;
			leftChild = LCHILD(curr);
		}
		return curr;
	} */
	inline size_t pushMaxDown(size_t curr) {
		auto leftChild = LCHILD(curr);
		while (leftChild < v.size()) {
			auto max = maxChild(curr);
			v[curr]= v[max];
			curr = max;
			leftChild = LCHILD(curr);
		}
		return curr;
	}
	inline size_t pushMinDown(size_t curr) {
		auto leftChild = LCHILD(curr);
		while (leftChild < v.size()) {
			auto min = minChild(curr);
			v[curr] = v[min];
			curr = min;
			leftChild = LCHILD(curr);
		}
		return curr;
	}
	inline void pushMinTree(size_t curr) {
		while (haveParent(curr)) {
			if (auto parent = PARENT(curr); v[curr].key < v[parent].key) {
				swap(curr, parent);
				curr = parent;
			}
			else break;
		}
	}
	inline void pushMaxTree(size_t curr) {
		while (haveParent(curr)) {
			if (auto parent = PARENT(curr); v[parent].key < v[curr].key) {
				swap(curr, parent);
				curr = parent;
			}
			else break;
		}
	}

	inline void pushUp(const size_t& curr) {
		auto [isMinLevel, correspodingIndex] = isMinLevelAndCorresponding(curr);
		if (isMinLevel) {// nowy element na stronie min
			if (v[curr].key < v[correspodingIndex].key) {
				pushMinTree(curr);
			}
			else {
				swap(curr, correspodingIndex);
				pushMaxTree(correspodingIndex);
			}
		}
		else {//strona max
			if (v[correspodingIndex].key < v[curr].key) {
				pushMaxTree(curr);
			}
			else {
				swap(curr, correspodingIndex);
				pushMinTree(correspodingIndex);
			}
		}
	}

public:
	Deap() {
		v.emplace_back();
	}
	~Deap() {}
	Element<T> getMin() {
		if (v.size() == 1)
			throw std::exception("heap is mpty");
		else
			return v[1];
	}
	Element<T> getMax() {
		if (v.size() == 1)
			throw std::exception("heap is empty");
		else if (v.size() == 2)
			return v[1];
		else
			return v[2];
	}
	void removeMin() {
		if (v.size() <= 3) {
			if (v.size() == 1) {
				throw std::exception("heap is empty");
			}
			else if (v.size() == 3) {// 2 elementy
				v[1] = v.back();
				v.pop_back();
			}
			else {// 1 element
				v.pop_back();
			}
			return;
		}
		auto tmp = v.back();
		v.pop_back();
		auto leafIndex = pushMinDown(1);
		v[leafIndex] = tmp;
		auto [_, correspodingIndex] = isMinLevelAndCorresponding(leafIndex);
		if (v[correspodingIndex].key < v[leafIndex].key) {
			swap(correspodingIndex, leafIndex);
			pushUp(correspodingIndex);
		}
		pushUp(leafIndex);
	}
	void removeMax() {
		if (v.size() <= 3) {
			if (v.size() == 1) {
				throw std::exception("heap is empty");
			}
			else {// 1 element
				v.pop_back();
			}
			return;
		}
		auto tmp = v.back();
		v.pop_back();
		auto leafIndex = pushMaxDown(2);
		v[leafIndex] = tmp;
		auto [_, correspodingIndex] = isMinLevelAndCorresponding(leafIndex);
		if (v[correspodingIndex].key < v[leafIndex].key) {
			swap(correspodingIndex, leafIndex);
			pushUp(correspodingIndex);
		}
		pushUp(leafIndex);
	}
	void put(Element<T>& e) {
		auto current = v.size();
		v.emplace_back(e);
		if (current == 1)	return; // dodalismy pirwszy element, struktura nie wymaga naprawy
		
		// mamy na ostatnim elemencie nowy element
		pushUp(current);
	}

	void dumpKeys() {
		std::cout << '\n';
		std::cout << '\n';
		std::cout << '\n';
		std::cout << '\n';
		if (!v.empty())
			print(v);
	}
	
	void dumpElementLevelAndCorresponding(const size_t& element) {
		auto [isMinLevel, correspodingIndex] = isMinLevelAndCorresponding(element);
		std::cout << "dla elementu " << element << '\n';
		std::cout << isMinLevel << " " << correspodingIndex << '\n';
	}
};

#endif