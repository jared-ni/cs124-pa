#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include <iostream>
void swap(float *x, float *y);

// a class for constructing binary heap
class MinHeap {
    
public:
    float *H; // pointer to array H: elements in heap
    int max_size; // size of H array
    int cur_size; // Current number of elements in min heap
    // constructor
    MinHeap(int capacity);
  
    // to heapify a subtree with the root at given index
    void heapify(int n);
  
    // to get parent_index node index of node at index i
    int parent_index(int i) { 
        return (i - 1) / 2; 
    }
    // to get index of node at index i's left child, or right child
    int right_index(int i) { 
        return (2 * i + 2); 
    }
    int left_index(int i) { 
        return (2 * i + 1); 
    }

    // Returns the minimum item from min heap
    float peek_min() { 
        return H[0]; 
    }
  
    // get and delete minimum element
    float extract_min();
  
    // inserts a new item
    void insert(float item);
};
  
// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(int cap) {
    cur_size = 0;
    max_size = cap;
    H = new float[cap];
}

// heapify subtree with root at index n
void MinHeap::heapify(int n) {
    int l = left_index(n);
    int r = right_index(n);
    int smallest = n;
    if (l < cur_size && H[l] < H[n])
        smallest = l;
    if (r < cur_size && H[r] < H[smallest])
        smallest = r;
    if (smallest != n)
    {
        swap(&H[n], &H[smallest]);
        heapify(smallest);
    }
}

// Method to remove minimum element (or root) from min heap
float MinHeap::extract_min() {
    // if heap is empty, return INT_MAX
    if (cur_size <= 0) { return INT_MAX; } 
    // if heap has only one element, return it
    if (cur_size == 1) { cur_size--; return H[0]; }

    // following section 2 pseudocode
    float min_ele = H[0];
    H[0] = H[cur_size-1];
    cur_size--;
    heapify(0);
    return min_ele;
}
  
// Inserts a new value v to heap
void MinHeap::insert(float v) {
    // check that heap is not full
    if (cur_size == max_size) { printf("\nHeap overflow\n"); return; }
  
    // following section 2 pseudocode
    cur_size++;
    int n = cur_size - 1;
    H[n] = v;
  
    while (n != 0 && H[parent_index(n)] > H[n]) {
       swap(&H[parent_index(n)], &H[n]);
       n = parent_index(n);
    }
}
  
// A utility function to swap two elements
void swap(float *x, float *y) {
    float temp = *x;
    *x = *y;
    *y = temp;
}


int main()
{
    MinHeap h(11);
    std::cout << "h.cur_size: " << h.cur_size << std::endl;
    for (int i = 10; i != 0; i--) {
        h.insert(i/10.0);
    }

    for (int i = 0; i < 10; i++) {
        std::cout << "extractmin: " << h.extract_min() << std::endl;
    }

    std::cout << "h.cur_size: " << h.cur_size << std::endl;

    // for (int i = 0; i < 10; i++) {
    //     std::cout << "h.H[i]: " << h.H[i] << std::endl;
    // }

    // push more values in
    for (int i = 10; i != 0; i--) {
        h.insert(i/10.0);
    }
    std::cout << "h.cur_size: " << h.cur_size << std::endl;

    for (int i = 0; i < 10; i++) {
        std::cout << "extractmin: " << h.extract_min() << std::endl;
    }
    std::cout << "h.cur_size: " << h.cur_size << std::endl;



    // h.insert(1.0);
    // h.insert(0.5);
    // h.insert(0.2);
    // h.insert(0.7);
    // float is2 = h.extract_min();
    // std::cout << "is2: " << is2 << std::endl;

    // h.insert(0.1);
    // float is1 = h.extract_min();
    // std::cout << "is1: " << is1 << std::endl;

    // float is5 = h.peek_min();
    // std::cout << "is5: " << is5 << std::endl;

    // float isStill5 = h.extract_min();
    // std::cout << "isStill5: " << isStill5 << std::endl;


    return 0;
}