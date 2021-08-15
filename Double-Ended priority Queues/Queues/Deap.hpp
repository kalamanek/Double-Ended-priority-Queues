#ifndef __DEAP_HPP
#define __DEAP_HPP


#include <vector>
#include <stdexcept>
#include <iostream>	
#include "QueueBase.hpp"


template<typename T>
class Deap : public QueueBase<T> {
	std::vector<Element<T>> v;
	size_t height;
	size_t size;

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
		if (lchild + 1 < size)// wszystkie dzieci			
			return v[lchild].key < v[lchild + 1].key ? lchild : lchild + 1;
		else
			return lchild;	 // tylko jeden element
	}
	inline size_t maxChild(const size_t& i) { //lchild musi istniec
		auto lchild = LCHILD(i);
		if (lchild + 1 < size)// wszystkie dzieci			
			return v[lchild + 1].key < v[lchild].key ? lchild : lchild + 1;
		else
			return lchild;	 // tylko jeden element
	}

	inline std::pair<bool, size_t> isMinLevelAndCorresponding(const size_t& i) {		// diala tylko do inisertu (ostatni element w drzewie zupelnym)
		//const auto level = elementLevel(i + 1);
		auto lastLevelBegin = (1 << height) - 1;
		const size_t level = lastLevelBegin <= i ? height : height - 1;
		//const size_t level = elementLevel(i);
		const size_t LevelIndexAmount = 1 << level;
		const size_t halfLevelIndexAmount = LevelIndexAmount / 2;

		const auto isMinLevel = (i < LevelIndexAmount - 1 + halfLevelIndexAmount ? true : false);
		
		if (isMinLevel) {//minlvl
			auto optional = i + halfLevelIndexAmount;
			if (optional < size) {// jezeli istnieje odpowiadajacy pozycji w max drzewku
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
		while (leftChild < size) {
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
		while (leftChild < size) {
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
		while (leftChild < size) {
			auto max = maxChild(curr);
			v[curr]= v[max];
			curr = max;
			leftChild = LCHILD(curr);
		}
		return curr;
	}
	inline size_t pushMinDown(size_t curr) {
		auto leftChild = LCHILD(curr);
		while (leftChild < size) {
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
		height = 0;
		size = 1;
	}
	~Deap() {}
	Element<T> getMin() {
		if (size == 1)
			throw std::exception("heap is mpty");
		else
			return v[1];
	}
	Element<T> getMax() {
		if (size == 1)
			throw std::exception("heap is empty");
		else if (size == 2)
			return v[1];
		else
			return v[2];
	}

	inline bool isPow2(size_t x) { return 0 == (x & (x - 1)); }
	inline void removeMin() {
		if (size <= 3) {
			if (size == 1) {
				throw std::exception("heap is empty");
			}
			else if (size == 3) {// 2 elementy
				v[1] = v.back();
				v.pop_back();
				size = 2;
			}
			else {// 1 element
				v.pop_back();
				height = 0;
				size = 1;
			}
			return;
		}
		auto tmp = v.back();
		if (isPow2(size)) --height;
		--size;
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
	inline void removeMax() {
		if (size <= 3) {
			if (size == 1) {
				throw std::exception("heap is empty");
			}
			else {// 1 lub 2 elementy
				v.pop_back();
				--size;
				height = size == 1 ? 0 : 1;
			}
			return;
		}
		auto tmp = v.back();
		if (isPow2(size)) --height;
		--size;
		v.pop_back();
		auto leafIndex = pushMaxDown(2);
		v[leafIndex] = tmp;
		auto [_, correspodingIndex] = isMinLevelAndCorresponding(leafIndex);
	
		if (auto lChild = LCHILD(correspodingIndex); lChild < size) 		  //max z dzieci o ile istnieja
			if (auto maxC = maxChild(correspodingIndex); v[correspodingIndex].key < v[maxC].key)
				correspodingIndex = maxC;
		

		if(v[leafIndex].key < v[correspodingIndex].key) {
			swap(correspodingIndex, leafIndex);
			pushUp(correspodingIndex);
		}
		pushUp(leafIndex);
	}
	void put(const Element<T>& e) {
		auto current = size;
		v.emplace_back(e);
		++size;
		if (isPow2(size)) ++height;
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