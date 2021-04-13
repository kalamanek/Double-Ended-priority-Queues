#ifndef __PRINTTREE_HPP
#define __PRINTTREE_HPP
#include <stdio.h>
#include <math.h> 
#include <vector>
#include <iostream>	


#define P(i) ((i-1) / 2)
#define LINE_WIDTH 280

template<typename T>
void print(const std::vector<Element<T>>& v) {
	size_t NUM_NODES = v.size();
    int * print_pos = new int[NUM_NODES];
    int i, j, k, pos, x = 1, level = 0;

    print_pos[0] = 0;
    for (i = 0, j = 1; i < NUM_NODES; i++, j++) {
        pos = (int) (print_pos[P(i)] + (i % 2 ? -1 : 1) * (LINE_WIDTH / (pow(2, level + 1)) + 1));

        for (k = 0; k < pos - x; k++) std::cout <<  (i == 0 || i % 2 ? ' ' : '-');
        std::cout << v[i].key;

        print_pos[i] = x = pos + 1;
        if (j == pow(2, level)) {
            printf("\n");
            level++;
            x = 1;
            j = 0;
        }
    }
    delete[] print_pos;
}
#endif
