#undef NDEBUG
#include <assert.h>


#include <iostream>
#include <chrono>
#include "Element.hpp"
#include "Queues/SymmetricMinMaxHeap.hpp"    
#include "Queues/IntervalHeap.hpp"
#include "Queues/MinMaxHeap.hpp"
#include "Queues/Deap.hpp"
#include <cassert>
#include <random>




std::mt19937 mt(time(0));


template <typename T>
std::vector<T> genRandom(int limit) {
    std::vector<T> tests;

    tests.reserve(limit);

    for (int i = 1; i <= limit; ++i)
        tests.push_back(i);

    std::shuffle(tests.begin(), tests.end(), mt);

    return tests;
}

std::vector<bool> testOnlyMin(int limit) {
    return std::vector<bool>(limit, true);
}

std::vector<bool> testOnlyMax(int limit) {
    return std::vector<bool>(limit, false);
}

std::vector<bool> testRandom(int limit) {
    std::vector<bool> tests;
    tests.reserve(limit);
    std::generate_n(std::back_inserter(tests), limit, []() { return !(mt() % 2); });
    return tests;
}

template<typename QueueBase, typename T>
void insertSample(QueueBase& q, const std::vector<T>& data) {
    for (const auto& x : data)
        q.put(Element<T> { size_t(x), 2 });
}

template<typename QueueBase, typename T>
void testSample(QueueBase& q, const std::vector<T>& data, const std::vector<bool>& tests) {
    assert(data.size() == tests.size());
    try {
        auto b = data.begin(), e = data.end();
        auto t = tests.begin();
        while (b != e) {
            // q.dumpKeys();
            //std::cout << std::endl;
            if (*t++) {
                //std::cout << "min " << *b << std::endl;
                assert(q.getMin().key == *b);
                ++b;
                q.removeMin();
            }
            else {
                --e;
                //std::cout << "max " << *e << std::endl;
                assert(q.getMax().key == *e);
                q.removeMax();
            }
        }
    }
    catch (std::exception e) {
        return;
    }
}
template<typename QueueBase, typename T>
void removeMinOrder(QueueBase& q, unsigned int limit = 15) {  //TEMPORARY TODO
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
template<typename QueueBase, typename T>
void removeMaxOrder(QueueBase& q, unsigned int limit = 15) {  //TEMPORARY TODO
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

template<typename QueueBase, typename T>
void test(QueueBase& q, const std::vector<T>& data, const std::vector<bool>& tests) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    std::vector<T> sorted;
    std::copy(data.begin(), data.end(), std::back_inserter(sorted));
    std::sort(sorted.begin(), sorted.end());

    auto t1 = high_resolution_clock::now();
    insertSample(q, data);
    auto t2 = high_resolution_clock::now();
    testSample(q, sorted, tests);
    //removeMaxOrder(q, limit);
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
    auto number = 1000000;

    for (int i = 0; i < 10; ++i) {
        auto data = genRandom<int>(number);
        auto tests = testOnlyMin(number);

        std::cout << "symetric\n";
        test(a, data, tests);
        std::cout << "deap\n";
        test(b, data, tests);
        std::cout << "interval\n";
        test(c, data, tests);
        std::cout << "minmax\n";
        test(d, data, tests);

    }

    return 0;
}