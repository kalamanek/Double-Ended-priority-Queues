#include <iostream>
#include <chrono>
#include "Element.hpp"
#include "Queues/SymmetricMinMaxHeap.hpp"    
#include "Queues/IntervalHeap.hpp"
#include "Queues/MinMaxHeap.hpp"
#include "Queues/Deap.hpp"
#include <cassert>

template<typename T>
void insertSample(QueueBase<T>& q , int limit = 15) {
    for (int i = limit; i > 0; i--) {
        auto e = Element<int>{ size_t(i), 2 };
        q.put(e);
    }
    //for (auto key : { 4, 80, 8, 60, 6, 40, 12, 20, 10, 16, 14, 30 }) {
/*  for (auto key : { 30,14,16,10,20,12,40,6,60,8,80,4}) {
      auto e = Element<int>{ size_t (key), 2 };
      a.put(e);
      a.dumpKeys();
  }*/
}

template<typename T>
void removeMinOrder(QueueBase<T>& q, unsigned int limit = 15) {  //TEMPORARY TODO
    try {
        for (int i = 1; i <= limit; i++) {
            //std::cout << '\n' << q.getMin().key << '\n';  
            assert(i == q.getMin().key);
            q.removeMin();
        }
    }
    catch (std::exception e) {
        return;
    }
}
template<typename T>
void removeMaxOrder(QueueBase<T>& q, unsigned int limit = 15) {  //TEMPORARY TODO
    try {
        for (int i = limit ; i > 0; i--) {
            //std::cout << '\n' << q.getMax().key << '\n';
            assert(i == q.getMax().key);
            q.removeMax();
        }
    }
    catch (std::exception e) {
        return;
    }
}

template<typename T>
void test(QueueBase<T>& q, unsigned int limit) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;


    auto t1 = high_resolution_clock::now();
    insertSample(q, limit);
    auto t2 = high_resolution_clock::now();
    removeMaxOrder(q, limit);
    auto t3 = high_resolution_clock::now();

    duration<double, std::milli> first = t2 - t1;
    duration<double, std::milli> second = t3 - t2;

    std::cout << "insert time: " << first.count() << "ms\n";
    std::cout << "remove time: " << second.count() << "ms\n";
}

int main() {
    auto a = SymmetricMinMaxHeap<int>();
    auto b = Deap<int>();
    auto c = IntervalHeap<int>();
    auto d = MinMaxHeap<int>();
    std::cout << "symetric\n";
    test<int>(a, 50000);
    std::cout << "deap\n";
    test<int>(b, 50000);
    std::cout << "interval\n";
    test<int>(c, 50000);
    std::cout << "minmax\n";
    test<int>(d, 50000);

    return 0;
}