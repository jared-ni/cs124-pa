#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <iostream>
#include <climits>
#include <set>
#include "minheap.h"
#include <cstdlib>
#include <ctime>
#include <thread>

using namespace std;

// make graph a pointer and not return it
int construct_graph0(int n, vector<tuple<int, float>> *adjList);
void construct_graph2(int n, vector<tuple<int, float>> *adjList);
void construct_graph3(int n, vector<tuple<int, float>> *adjList);
void construct_graph4(int n, vector<tuple<int, float>> *adjList);
float prim(vector<tuple<int, float>> *graph, int n, int graphSize);
float rand_num();

void dimension_trial(int n, int dimensions);

// ./randmst 0 numpoints numtrials dimension
int main(int argc, char *argv[])
{
    // establish parameters
    int flexibility = atoi(argv[1]);
    // test numpoints = 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144
    int numpoints = atoi(argv[2]);
    int numtrials = atoi(argv[3]);
    int dimension = atoi(argv[4]);

    srand(static_cast<unsigned>(time(0)));

    // construct graph dimension 0
    dimension_trial(numpoints, dimension);

    // std::thread dimension_thread1(dimension_trial, numtrials, 0);
    // dimension_thread1.join();

    return 0;
}

// dimension_trial0
void dimension_trial(int n, int dimensions)
{
    // construct graph dimension 0
    vector<tuple<int, float>> graph[n];
    vector<tuple<int, float>> *vList;
    vList = graph;
    int graphSize = 0;
    if (dimensions == 0)
    {
        graphSize = construct_graph0(n, vList);
    }
    else if (dimensions == 2)
    {
        construct_graph2(n, vList);
    }
    else if (dimensions == 3)
    {
        construct_graph3(n, vList);
    }
    else if (dimensions == 4)
    {
        construct_graph4(n, vList);
    }
    float total_weight = prim(graph, n, graphSize);
    cout << "Dimension: " << dimensions << ", n: " << n << ", total weight: " << total_weight << endl;
}

// use Prim's Algorithm to find minimum spanning tree using our minHeap
// returns the total weight of the MST
float prim(vector<tuple<int, float>> *graph, int n, int graphSize)
{
    float total_weight = 0.0;
    set<int> S;
    MinHeap H = MinHeap(graphSize);
    float dist[n];
    // initialize dist to infinity (2.0)
    for (int i = 0; i < n; i++)
    {
        dist[i] = 100000.0;
    }
    dist[0] = 0.0;
    // insert starting vertex into H
    H.insert(make_tuple(0, 0.0));
    while (H.cur_size != 0)
    {
        tuple<int, float> v = H.extract_min();
        // cout << "v: " << get<0>(v) << ", weight: " << get<1>(v) << endl;

        S.insert(get<0>(v));
        // for each neighbor of v
        for (tuple<int, float> neighbor : *(graph + get<0>(v)))
        {
            // if neighbor is not in S, add it to H
            if (dist[get<0>(neighbor)] > get<1>(neighbor) && S.find(get<0>(neighbor)) == S.end())
            {
                dist[get<0>(neighbor)] = get<1>(neighbor);
                H.insert(neighbor);
            }
        }
    }
    // calculate total weight
    for (int i = 0; i < n; i++)
    {
        total_weight += dist[i];
    }
    return total_weight;
}

// returns random number between 0 and 1
float rand_num()
{
    return ((float)rand()) / RAND_MAX;
}

// construct graph for dimension 1 with n nodes
int construct_graph0(int n, vector<tuple<int, float>> *vList)
{
    int c = 0;
    // loop through all nodes
    for (int i = 0; i < n; i++)
    {
        // vector<tuple<int, float>> current = *(vList + i);
        // loop through all nodes again
        for (int j = 0; j < i; j++)
        {
            // add edge to vList
            float randnum = rand_num();
            if ((n <= 100) || (n > 100 && n < 1000 && randnum < 0.2) || (n >= 1000 && n < 10000 && randnum < 0.1) || (n >= 10000 && randnum < 0.01))
            {
                (*(vList + i)).push_back(make_tuple(j, randnum));
                (vList + j)->push_back(make_tuple(i, randnum));
                c++;
            }
        }
        if (i % 1000 == 0)
        {
            cout << "i: " << i << endl;
        }
    }
    return c;
}

// construct graph for dimension 2 with n nodes
void construct_graph2(int n, vector<tuple<int, float>> *vList)
{
    // get coordinates
    tuple<float, float> coordinates[n];
    for (int i = 0; i < n; i++)
    {
        float a = rand_num();
        float b = rand_num();
        coordinates[i] = make_tuple(a, b);
    }
    // loop through all nodes
    for (int i = 0; i < n; i++)
    {
        // loop through all nodes again
        for (int j = 0; j < n; j++)
        {
            // add edge to vList
            if (i != j)
            {
                float weight = sqrt(pow(get<0>(coordinates[i]) - get<0>(coordinates[j]), 2) +
                                    pow(get<1>(coordinates[i]) - get<1>(coordinates[j]), 2));
                // this check is only accurate for n >= 100
                if (n <= 100 || weight < 0.4)
                {
                    (vList + i)->push_back(make_tuple(j, weight));
                }
            }
        }
    }
}

// construct graph for dimension 3 with n nodes
void construct_graph3(int n, vector<tuple<int, float>> *vList)
{
    // get coordinates
    tuple<float, float, float> coordinates[n];
    for (int i = 0; i < n; i++)
    {
        float a = rand_num();
        float b = rand_num();
        float c = rand_num();
        coordinates[i] = make_tuple(a, b, c);
    }
    // loop through all nodes
    for (int i = 0; i < n; i++)
    {
        // loop through all nodes again
        for (int j = 0; j < n; j++)
        {
            // add edge to vList
            if (i != j)
            {
                float weight = sqrt(pow(get<0>(coordinates[i]) - get<0>(coordinates[j]), 2) +
                                    pow(get<1>(coordinates[i]) - get<1>(coordinates[j]), 2) +
                                    pow(get<2>(coordinates[i]) - get<2>(coordinates[j]), 2));
                // this check is only accurate for n > 100
                if (n <= 100 || weight < 0.6)
                {
                    (vList + i)->push_back(make_tuple(j, weight));
                }
            }
        }
    }
}

// construct graph for dimension 4 with n nodes
void construct_graph4(int n, vector<tuple<int, float>> *vList)
{
    // get coordinates
    tuple<float, float, float, float> coordinates[n];
    for (int i = 0; i < n; i++)
    {
        float a = rand_num();
        float b = rand_num();
        float c = rand_num();
        float d = rand_num();
        coordinates[i] = make_tuple(a, b, c, d);
    }
    // loop through all nodes
    for (int i = 0; i < n; i++)
    {
        // loop through all nodes again
        for (int j = 0; j < n; j++)
        {
            // add edge to vList
            if (i != j)
            {
                float weight = sqrt(pow(get<0>(coordinates[i]) - get<0>(coordinates[j]), 2) +
                                    pow(get<1>(coordinates[i]) - get<1>(coordinates[j]), 2) +
                                    pow(get<2>(coordinates[i]) - get<2>(coordinates[j]), 2) +
                                    pow(get<3>(coordinates[i]) - get<3>(coordinates[j]), 2));
                // this check is only accurate for n > 100
                if (n <= 100 || weight < 0.8)
                {
                    (vList + i)->push_back(make_tuple(j, weight));
                }
            }
        }
    }
}
