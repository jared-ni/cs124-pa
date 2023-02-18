#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <tuple>
#include<iostream>
#include<climits>
#include <optional>

using namespace std;

// swap function for priority queue
void swap(float *x, float *y);

// ./randmst 0 numpoints numtrials dimension
int main(int argc,char* argv[]) {
    printf("Number of arguments: %i\n", argc);
    printf("Arguments are: %s\n", argv[0]);

    // establish parameters
    int flexibility = atoi(argv[1]);
    int numpoints = atoi(argv[2]);
    int numtrials = atoi(argv[3]);
    int dimension = atoi(argv[4]);
    
    // construct graph depending on dimension
    unordered_map<int, vector<tuple<int, float>>> graph = construct_graph(numpoints, dimension);

    // get minimum spanning tree using Prim's Algorithm



    return 0;
}

// implement priority queue



// use Prim's Algorithm to find minimum spanning tree
// returns the total weight of the MST
float prim_1d(unordered_map<int, vector<int>> graph) {
    // TODO: implement Prim's Algorithm
    
}

// randomizes vertices for dimension 2
unordered_map<int, vector<tuple<float, float>>> vertex_dict2(int n) {
    unordered_map<int, vector<tuple<float, float>>> vertices;
    for (int i = 0; i < n; i++) {
        vertices[i] = vector<tuple<float, float>>();
        vertices[i].push_back(make_tuple(rand_num(), rand_num()));
    }
    return vertices;
}

// randomizes vertices for dimension 3
unordered_map<int, vector<tuple<float, float, float>>> vertex_dict3(int n) {
    unordered_map<int, vector<tuple<float, float, float>>> vertices;
    for (int i = 0; i < n; i++) {
        vertices[i] = vector<tuple<float, float, float>>();
        vertices[i].push_back(make_tuple(rand_num(), rand_num(), rand_num()));
    }
    return vertices;
}

// randomizes vertices for dimension 4
unordered_map<int, vector<tuple<float, float, float, float>>> vertex_dict4(int n) {
    unordered_map<int, vector<tuple<float, float, float, float>>> vertices;
    for (int i = 0; i < n; i++) {
        vertices[i] = vector<tuple<float, float, float, float>>();
        vertices[i].push_back(make_tuple(rand_num(), rand_num(), rand_num(), rand_num()));
    }
    return vertices;
}

// returns random number between 0 and 1
float rand_num() {
    float weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return weight;
}

// calculates edge weight for dimension 2
float calc_weight2(int i, int j, unordered_map<int, vector<tuple<float, float>>> vertices) {
    float weight = sqrt(pow(get<0>(vertices[i].front()) - get<0>(vertices[j].front()), 2) + pow(get<1>(vertices[i].front()) - get<1>(vertices[j].front()), 2));
    return weight;
}

// calculates edge weight for dimension 3
float calc_weight3(int i, int j, unordered_map<int, vector<tuple<float, float, float>>> vertices) {
    float weight = sqrt(pow(get<0>(vertices[i].front()) - get<0>(vertices[j].front()), 2) + pow(get<1>(vertices[i].front()) - get<1>(vertices[j].front()), 2) + pow(get<2>(vertices[i].front()) - get<2>(vertices[j].front()), 2));
    return weight;
}

// calculates edge weight for dimension 4
float calc_weight4(int i, int j, unordered_map<int, vector<tuple<float, float, float, float>>> vertices) {
    float weight = sqrt(pow(get<0>(vertices[i].front()) - get<0>(vertices[j].front()), 2) + pow(get<1>(vertices[i].front()) - get<1>(vertices[j].front()), 2) + pow(get<2>(vertices[i].front()) - get<2>(vertices[j].front()), 2) + pow(get<3>(vertices[i].front()) - get<3>(vertices[j].front()), 2));
    return weight;
}

// construct graph with n nodes and randomized weights for dimension n
unordered_map<int, vector<tuple<int, float>>> construct_graph(int n, int dimension) {
    unordered_map<int, vector<tuple<float, float>>> vertices2 = {};
    unordered_map<int, vector<tuple<float, float, float>>> vertices3 = {};
    unordered_map<int, vector<tuple<float, float, float, float>>> vertices4 = {};
    if (dimension == 2) {
        vertices2 = vertex_dict2(n);
    }
    else if (dimension == 3) {
        vertices3 = vertex_dict3(n);
    }
    else if (dimension == 4) {
        vertices4 = vertex_dict4(n);
    }
    unordered_map<int, vector<tuple<int, float>>> graph;
    for (int i=0; i<n; i++) {
        graph[i] = vector<tuple<int, float>>();
        for (int j=0; j<n; j++) {
            if (i != j) {
                if (dimension == 0) {
                    graph[i].push_back(make_tuple(j, rand_num()));
                }
                else if (dimension == 2) {
                    graph[i].push_back(make_tuple(j, calc_weight2(i, j, vertices2)));
                }
                else if (dimension == 3) {
                    graph[i].push_back(make_tuple(j, calc_weight3(i, j, vertices3)));
                }
                else if (dimension == 4) {
                    graph[i].push_back(make_tuple(j, calc_weight4(i, j, vertices4)));
                }
            }
        }
    } 

    return graph;     
}




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
MinHeap::MinHeap(int cap)
{
    cur_size = 0;
    max_size = cap;
    H = new float[cap];
}

// heapify subtree with root at index n
void MinHeap::heapify(int n)
{
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
float MinHeap::extract_min()
{
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
void MinHeap::insert(float v)
{
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
void swap(float *x, float *y)
{
    float temp = *x;
    *x = *y;
    *y = temp;
}