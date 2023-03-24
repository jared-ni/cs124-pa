#include <climits>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <tuple>

using namespace std;

// swap function for priority queue
void swap(tuple<int, float> *x, tuple<int, float> *y);


// a class for constructing binary heap
class MinHeap {
    
public:
    // first is vertex, second is weight
    tuple<int, float> *H; // pointer to array H: elements in heap

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
    tuple<int, float> peek_min() { 
        return H[0]; 
    }
  
    // get and delete minimum element
    tuple<int, float> extract_min();
  
    // inserts a new item
    void insert(tuple<int, float> item);
};
  
// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(int cap) {
    cur_size = 0;
    max_size = cap;
    H = new tuple<int, float>[cap];
}

// heapify subtree with root at index n
void MinHeap::heapify(int n) {
    int l = left_index(n);
    int r = right_index(n);
    int smallest = n;
    if (l < cur_size && get<1>(H[l]) < get<1>(H[n]))
        smallest = l;
    if (r < cur_size && get<1>(H[r]) < get<1>(H[smallest]))
        smallest = r;
    if (smallest != n)
    {
        swap(&H[n], &H[smallest]);
        heapify(smallest);
    }
}

// Method to remove minimum element (or root) from min heap
tuple<int, float> MinHeap::extract_min() {
    // if heap is empty, return INT_MAX
    if (cur_size <= 0) { return make_tuple(INT_MAX, INT_MAX); } 
    // if heap has only one element, return it
    if (cur_size == 1) { cur_size--; return H[0]; }

    // following section 2 pseudocode
    tuple<int, float> min_ele = H[0];
    H[0] = H[cur_size-1];
    cur_size--;
    heapify(0);
    return min_ele;
}
  
// Inserts a new value v to heap
void MinHeap::insert(tuple<int, float> v) {
    // check that heap is not full
    if (cur_size == max_size) { printf("\nHeap overflow\n"); return; }
  
    // following section 2 pseudocode
    cur_size++;
    int n = cur_size - 1;
    H[n] = v;
  
    while (n != 0 && get<1>(H[parent_index(n)]) > get<1>(H[n])) {
       swap(&H[parent_index(n)], &H[n]);
       n = parent_index(n);
    }
}
  
// A utility function to swap two elements
void swap(tuple<int, float> *x, tuple<int, float> *y) {
    tuple<int, float> temp = *x;
    *x = *y;
    *y = temp;
}