#include <iostream>
#include <vector>
#include "../Headers/KeyVal.hpp"
#include "../Headers/BinaryHeap.hpp"

/**
 * @class BinaryHeap
 * @brief This is an implementation of a priority queue using a binary heap data structure.
 *        A binary heap is a complete binary tree with a special property where the key of
 *        each node is either always greater than or always less than the keys of its children,
 *        depending on whether it is a max heap or a min heap.
 * 
 * @param initialMaxSize The amount of room preallocated for the binary heap.
 * @param isMaxHeap A boolean value indicating whether the top of the heap is a maximum or minimum value.
 * 
 */
BinaryHeap::BinaryHeap(int initialMaxSize, bool isMaxHeap) : numInHeap(0), isMaxHeap(isMaxHeap) {
    heapArray.reserve(initialMaxSize);
}

BinaryHeap::~BinaryHeap() {
    // No explicit memory deallocation needed for std::vector
}

int BinaryHeap::heapSize() const {
    return numInHeap;
}

bool BinaryHeap::isEmpty() const {
    return numInHeap == 0;
}

void BinaryHeap::buildHeapFromKeysData(const std::vector<int>& keyArray, const std::vector<int>& dataArray) {
    numInHeap = keyArray.size();
    heapArray.resize(numInHeap);

    for (int curKey = 0; curKey < numInHeap; ++curKey) {
        heapArray[curKey] = KeyVal(keyArray[curKey], dataArray[curKey]);
    }

    int idx = numInHeap / 2;

    while (idx > 0) {
        percolateDown(idx);
        --idx;
    }
}

void BinaryHeap::insert(int key, int value) {
    ++numInHeap;
    int hole = numInHeap;

    if (isMaxHeap) {
        while (hole > 1 && key > heapArray[(hole / 2) - 1].getKey()) {
            heapArray[hole - 1] = heapArray[(hole / 2) - 1];
            hole /= 2;
        }
    } else {
        while (hole > 1 && key < heapArray[(hole / 2) - 1].getKey()) {
            heapArray[hole - 1] = heapArray[(hole / 2) - 1];
            hole /= 2;
        }
    }

    heapArray[hole - 1] = KeyVal(key, value);
}

KeyVal BinaryHeap::getTop() {
    if (numInHeap > 0) {
        return heapArray[0];
    } else {
        return KeyVal();
    }
}

KeyVal BinaryHeap::deleteTop() {
    if (numInHeap == 0) {
        return KeyVal();
    }

    KeyVal val = heapArray[0];
    heapArray[0] = heapArray[numInHeap - 1];
    --numInHeap;

    percolateDown(1);

    return val;
}

void BinaryHeap::percolateDown(int hole) {
    KeyVal temp = heapArray[hole - 1];

    if (isMaxHeap) {
        while (2 * hole <= numInHeap) {
            int child = 2 * hole;

            if (child != numInHeap && heapArray[child].getKey() < heapArray[child - 1].getKey()) {
                ++child;
            }

            if (heapArray[child - 1].getKey() > temp.getKey()) {
                heapArray[hole - 1] = heapArray[child - 1];
            } else {
                break;
            }

            hole = child;
        }
    } else {
        while (2 * hole <= numInHeap) {
            int child = 2 * hole;

            if (child != numInHeap && heapArray[child].getKey() < heapArray[child - 1].getKey()) {
                ++child;
            }

            if (heapArray[child - 1].getKey() < temp.getKey()) {
                heapArray[hole - 1] = heapArray[child - 1];
            } else {
                break;
            }

            hole = child;
        }
    }

    heapArray[hole - 1] = temp;
}

