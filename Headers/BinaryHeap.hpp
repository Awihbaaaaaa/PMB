#ifndef BINARYHEAP_H
#define BINARYHEAP_H

class BinaryHeap {
public:
    BinaryHeap(int initialMaxSize = 1, bool isMaxHeap = true);
    ~BinaryHeap();

    int heapSize() const;
    bool isEmpty() const;
    void buildHeapFromKeysData(const std::vector<int>& keyArray, const std::vector<int>& dataArray);
    void insert(int key, int value);
    KeyVal getTop();
    KeyVal deleteTop();

private:
    void percolateDown(int hole);

    int numInHeap;
    std::vector<KeyVal> heapArray;
    bool isMaxHeap;
};

#endif
