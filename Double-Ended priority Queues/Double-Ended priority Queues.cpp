#include <iostream>
#include "Element.hpp"
#include "Queues/SymmetricMinMaxHeap.hpp"


int main() {

    auto a = SymmetricMinMaxHeap<int>();

    //for (auto key : { 4, 80, 8, 60, 6, 40, 12, 20, 10, 16, 14, 30 }) {
 /*   for (auto key : { 30,14,16,10,20,12,40,6,60,8,80,4}) {
        auto e = Element<int>{ size_t (key), 2 };
        a.put(e);
        a.dumpKeys();
    }*/
    for (int i = 0; i <= 49; i++) {
        auto e = Element<int>{ size_t(i), 2 };
        a.put(e);
      }
    a.dumpKeys();


 //   std::cout << a.getMin().key << '\n';
 //   a.removeMin();
 //   a.dumpKeys();
    for (int i = 0; i < 50; i++) {
        std::cout << '\n' << a.getMax().key << '\n';
        a.removeMax();
        a.dumpKeys();
    }


//    auto e = Element<int>{ 2, 2 };
//    a.put(e);
//    a.dumpKeys();
}