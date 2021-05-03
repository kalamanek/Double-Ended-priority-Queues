#include <iostream>
#include "Element.hpp"
#include "Queues/SymmetricMinMaxHeap.hpp"    
#include "Queues/IntervalHeap.hpp"
#include "Queues/MinMaxHeap.hpp"

template<typename T>
void insertSample(QueueBase<T>& q , int limit = 15) {
    for (int i = 0; i < limit; i++) {
        auto e = Element<int>{ size_t(i), 2 };
        q.put(e);
       // q.dumpKeys();
    }
    //for (auto key : { 4, 80, 8, 60, 6, 40, 12, 20, 10, 16, 14, 30 }) {
/*  for (auto key : { 30,14,16,10,20,12,40,6,60,8,80,4}) {
      auto e = Element<int>{ size_t (key), 2 };
      a.put(e);
      a.dumpKeys();
  }*/
}

template<typename T>
void removeMinOrder(QueueBase<T>& q, int limit = 15) {  //TEMPORARY TODO
    try {
        for (int i = 0; i < limit; i++) {
            std::cout << '\n' << q.getMin().key << '\n';
            q.removeMin();
            q.dumpKeys();
        }
    }
    catch (std::exception e) {
        return;
    }
}
template<typename T>
void removeMaxOrder(QueueBase<T>& q, int limit = 15) {  //TEMPORARY TODO
    try {
        for (int i = 0; i < limit; i++) {
            std::cout << '\n' << q.getMax().key << '\n';
            q.removeMax();
            q.dumpKeys();
        }
    }
    catch (std::exception e) {
        return;
    }
}


int main() {

    auto a = MinMaxHeap<int>();
    insertSample(a,50);
    a.dumpKeys();
    removeMaxOrder(a,50);

}