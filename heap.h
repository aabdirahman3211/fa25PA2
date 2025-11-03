//
// Created by Manju Muralidharan on 10/19/25.
//

#ifndef HEAP_H
#define HEAP_H

#include <iostream>
using namespace std;

struct MinHeap {
    int data[64];
    int size;

    MinHeap() { size = 0; }

    static bool less(int a, int b, int weightArr[]) {
        if (weightArr[a] != weightArr[b]) return weightArr[a] < weightArr[b];
        return a < b;
    }
 // insert a new index into the heap at the end
    void push(int idx, int weightArr[]) {
        if (size >= 64) {
            cout << "Heap Overflow" << endl;
            return;
        }
        data[size] = idx;
        upheap(size, weightArr);
        ++size;
    }
// remove and return the smallest index
    int pop(int weightArr[]) {
        if (size == 0) {
            cout << "Heap Underflow" << endl;
            return -1;
        }
        int minIdx = data[0];
        data[0] = data[size-1];
        --size;
        if (size > 0) {downheap(0,weightArr);}
        return minIdx;
    }
    //Move an element upward until the heap property is satisfied.
    void upheap(int pos, int weightArr[]) {
        while (pos > 0) {
            int parent = (pos - 1) / 2;
            if (less(data[pos], data[parent], weightArr)) {
                swap(data[pos], data[parent]);
                pos = parent;
            }else {break;}
        }
    }
    // move an element downward until it is smaller than its children.
    void downheap(int pos, int weightArr[]) {
        while (true) {
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;
            int small = pos;
            if (left < size && less(data[left], data[small], weightArr)) {
                small = left;
            }
            if (right < size && less(data[right], data[small], weightArr)) {
                small = right;
            }

            if (small != pos) {
                swap(data[pos], data[small]);
                pos = small;
            } else {break;}
        }
    }
};

#endif