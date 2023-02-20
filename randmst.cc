#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <iostream>
#include <climits>
#include <set>
#include "minheap.h"

using namespace std;

unordered_map<int, vector<tuple<int, float>>> construct_graph(int n, int dimension);
float prim(unordered_map<int, vector<tuple<int, float>>> graph);
float rand_num();

// ./randmst 0 numpoints numtrials dimension
int main(int argc,char* argv[]) {
    // printf("Number of arguments: %i\n", argc);
    // printf("Arguments are: %s\n", argv[0]);

    // // establish parameters
    // int flexibility = atoi(argv[1]);
    // // test numpoints = 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 
    // int numpoints = atoi(argv[2]);
    // int numtrials = atoi(argv[3]); 
    // int dimension = atoi(argv[4]);
    
    // construct graph depending on dimension
    // run Prim's Algorithm
    unordered_map<int, vector<tuple<int, float>>> graph;
    graph[0] = vector<tuple<int, float>>();
    graph[0].push_back(make_tuple(1, 0.0));
    graph[0].push_back(make_tuple(2, 0.3));
    graph[0].push_back(make_tuple(3, 0.4));
    graph[1] = vector<tuple<int, float>>();
    graph[1].push_back(make_tuple(0, 0.0));
    graph[1].push_back(make_tuple(2, 0.5));
    graph[1].push_back(make_tuple(3, 0.6));
    graph[2] = vector<tuple<int, float>>();
    graph[2].push_back(make_tuple(0, 0.3));
    graph[2].push_back(make_tuple(1, 0.5));
    graph[2].push_back(make_tuple(3, 0.7));
    graph[3] = vector<tuple<int, float>>();
    graph[3].push_back(make_tuple(0, 0.4));
    graph[3].push_back(make_tuple(1, 0.6));
    graph[3].push_back(make_tuple(2, 0.7));

    float total_weight = prim(graph);
    cout << "total weight: " << total_weight << endl;

    // unordered_map<int, vector<tuple<int, float>>> graph = construct_graph(numpoints, dimension);
    // int avg_weight = 0.0;
    // for (int i = 0; i < numpoints; i++) {
    //     unordered_map<int, vector<tuple<int, float>>> graph = construct_graph(numpoints, dimension);
    //     avg_weight += prim(graph);
    // }
    // avg_weight /= numtrials;
    // cout << avg_weight;

    return 0;
}

// use Prim's Algorithm to find minimum spanning tree using our minHeap
// returns the total weight of the MST

/*
PrimMST(graph G)
    for each vertex v in G:
        v.distance = infinity
    start = arbitrary vertex
    start.distance = 0
    priorityQueue = all vertices in G
    while priorityQueue is not empty:
        u = vertex with smallest distance in priorityQueue
        remove u from priorityQueue
        for each neighbor v of u:
            if v is in priorityQueue and weight(u, v) < v.distance:
                v.distance = weight(u, v)
                v.parent = u
    return MST
*/

float prim(unordered_map<int, vector<tuple<int, float>>> graph) {
    float total_weight = 0.0;
    set<int> S;
    MinHeap H = MinHeap(graph.size() * (graph.size() - 1) / 2);
    float dist[graph.size()];
    cout << "graph size: " << graph.size() << endl;
    // initialize dist to infinity (2.0)
    for (int i = 0; i < graph.size(); i++) {

        dist[i] = 2.0;
    }
    dist[0] = 0.0;
    // insert starting vertex into H
    H.insert(make_tuple(0, 0.0));
    while (H.cur_size != 0) {
        tuple<int, float> v = H.extract_min();
        S.insert(get<0>(v));
        // for each neighbor of v
        for (tuple<int, float> neighbor : graph[get<0>(v)]) {
            // if neighbor is not in S, add it to H
            if (dist[get<0>(neighbor)] > get<1>(neighbor)) {
                dist[get<0>(neighbor)] = get<1>(neighbor);
                H.insert(neighbor);
            }
        }
    }
    // calculate total weight
    for (int i = 0; i < graph.size(); i++) {
        total_weight += dist[i];
        cout << "current weight: " << total_weight << endl;
    }
    return total_weight;
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
    float weight = sqrt(pow(get<0>(vertices[i].front()) - get<0>(vertices[j].front()), 2) + 
                        pow(get<1>(vertices[i].front()) - get<1>(vertices[j].front()), 2));
    return weight;
}

// calculates edge weight for dimension 3
float calc_weight3(int i, int j, unordered_map<int, vector<tuple<float, float, float>>> vertices) {
    float weight = sqrt(pow(get<0>(vertices[i].front()) - get<0>(vertices[j].front()), 2) + 
                        pow(get<1>(vertices[i].front()) - get<1>(vertices[j].front()), 2) + 
                        pow(get<2>(vertices[i].front()) - get<2>(vertices[j].front()), 2));
    return weight;
}

// calculates edge weight for dimension 4
float calc_weight4(int i, int j, unordered_map<int, vector<tuple<float, float, float, float>>> vertices) {
    float weight = sqrt(pow(get<0>(vertices[i].front()) - get<0>(vertices[j].front()), 2) + 
                        pow(get<1>(vertices[i].front()) - get<1>(vertices[j].front()), 2) + 
                        pow(get<2>(vertices[i].front()) - get<2>(vertices[j].front()), 2) + 
                        pow(get<3>(vertices[i].front()) - get<3>(vertices[j].front()), 2));
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