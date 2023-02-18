#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <tuple>

using namespace std;

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
    // ADD HERE 

    // get minimum spanning tree using Prim's Algorithm



    return 0;
}

// implement priority queue



// use Prim's Algorithm to find minimum spanning tree
// returns the total weight of the MST
float prim_1d(unordered_map<int, vector<int>> graph) {
    // TODO: implement Prim's Algorithm
    
}

float rand_num() {
    float weight = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return weight;
}

// construct graph with n nodes and randomized weights for dimension 0
unordered_map<int, vector<tuple<int, float>>> construct_graph1(int n) {
    
    unordered_map<int, vector<tuple<int, float>>> graph;
    for (int i=0; i<n; i++) {
        graph[i] = vector<tuple<int, float>>();
        for (int j=0; j<n; j++) {
            if (i != j) {
                graph[i].push_back(make_tuple(j, rand_num()));
            }
        }
    } 

    return graph;     
}

// construct graph with n nodes and randomized weights for dimension 2
unordered_map<int, vector<tuple<int, float>>> construct_graph2(int n) {
    
    unordered_map<int, vector<tuple<int, float>>> graph;
    for (int i=0; i<n; i++) {
        // need to do some math here ughghgh
        for (int j=0; j<n; j++) {
            if (i != j) {
                // wtf do i do here
                float dist = dist;
                graph[i].push_back(make_tuple(j, dist));
            }
        }
    } 

    return graph;     
}