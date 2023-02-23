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

float prim2(int n, int dimension);
float prim3(int n, int dimension);
float prim4(int n, int dimension);

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
    
    dimension_trial(numpoints, dimension);


    // run trials
    // for (int i = 0; i < numtrials; i++)
    // {
    //     dimension_trial(numpoints, dimension);
    // }

    return 0;
}


// dimension_trial0
void dimension_trial(int n, int dimensions)
{
    auto start = chrono::high_resolution_clock::now();

    float total_weight = 0;
    if (dimensions == 2)
        total_weight = prim2(n, dimensions);
    else if (dimensions == 3)
        total_weight = prim3(n, dimensions);
    else if (dimensions == 4)
        total_weight = prim4(n, dimensions);

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "\nThread[" << this_thread::get_id() << "]: Dimension: " << dimensions << ", n: " << n 
         << ", total weight: " << total_weight << ", runtime: " << duration.count() << " ms" << endl;
}


// use Prim's Algorithm to find minimum spanning tree using our minHeap
// returns the total weight of the MST
float prim2(int n, int dimension)
{
    cout << "starting prim2 on dimension " << dimension << ", n = " << n << endl;
    int graphSize = n * (n - 1) / 2;

    // create coordinates graph
    vector<tuple<float, float>> coordinates2;
    for (int i = 0; i < n; i++)
        coordinates2.push_back(make_tuple(rand_num(), rand_num()));

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
        int i = 0;
        for (auto& coordinate : coordinates2)
        {
            // if neighbor is v, skip
            if (i == get<0>(v)) {
                i++;
                continue;
            }
            // calculate weight of the edge between v and w
            float weight = 100000.0;
            weight = sqrt(pow(get<0>(coordinate) - get<0>(coordinates2[get<0>(v)]), 2) +
                          pow(get<1>(coordinate) - get<1>(coordinates2[get<0>(v)]), 2));
            
            // if ((n <= 100) || (n > 100 && n < 1000 && weight < 0.4) || (n >= 1000 && n < 10000 && weight < 0.1) || (n >= 10000 && weight < 0.05)) 
            // {
            if (dist[i] > weight && S.find(i) == S.end())
            {
                dist[i] = weight;
                H.insert(make_tuple(i, weight));
            }
            // }
            i++;
            // if neighbor is not in S, add it to H
        }
    }
    float total_weight = 0.0;
    // calculate total weight
    for (int i = 0; i < n; i++)
    {
        cout << "dist[" << i << "]: " << dist[i] << endl;
        total_weight += dist[i];
    }
    return total_weight;
}


// prim for dimension 3
float prim3(int n, int dimension)
{
    int graphSize = n * (n - 1) / 2;

    // create coordinates graph
    vector<tuple<float, float, float>> coordinates3;
    for (int i = 0; i < n; i++)
        coordinates3.push_back(make_tuple(rand_num(), rand_num(), rand_num()));

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
        int i = 0;
        for (auto& coordinate : coordinates3)
        {
            // if neighbor is v, skip
            if (i == get<0>(v)) { i++; continue; }
            // calculate weight of the edge between v and w
            float weight = 100000.0;

            weight = sqrt(pow(get<0>(coordinate) - get<0>(coordinates3[get<0>(v)]), 2) +
                          pow(get<1>(coordinate) - get<1>(coordinates3[get<0>(v)]), 2) +
                          pow(get<2>(coordinate) - get<2>(coordinates3[get<0>(v)]), 2));

            if ((n <= 100) || (n > 100 && n < 1000 && weight < 0.6) || (n >= 1000 && n < 10000 && weight < 0.3) || (n >= 10000 && weight < 0.1)) {
                // if neighbor is not in S, add it to H
                if (dist[i] > weight && S.find(i) == S.end())
                {
                    dist[i] = weight;
                    H.insert(make_tuple(i, weight));
                }
            }
            i++;
        }
    }
    float total_weight = 0.0;
    // calculate total weight
    for (int i = 0; i < n; i++)
    {
        total_weight += dist[i];
    }
    return total_weight;
}


// prim for dimension 4
float prim4(int n, int dimension)
{
    int graphSize = n * (n - 1) / 2;

    // create coordinates graph
    vector<tuple<float, float, float, float>> coordinates4;
    for (int i = 0; i < n; i++)
        coordinates4.push_back(make_tuple(rand_num(), rand_num(), rand_num(), rand_num()));

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
        int i = 0;
        for (auto& coordinate : coordinates4)
        {
            // if neighbor is v, skip
            if (i == get<0>(v)) { i++; continue; }
            
            // calculate weight of the edge between v and w
            float weight = 100000.0;
            weight = sqrt(pow(get<0>(coordinate) - get<0>(coordinates4[get<0>(v)]), 2) +
                          pow(get<1>(coordinate) - get<1>(coordinates4[get<0>(v)]), 2) +
                          pow(get<2>(coordinate) - get<2>(coordinates4[get<0>(v)]), 2) +
                          pow(get<3>(coordinate) - get<3>(coordinates4[get<0>(v)]), 2));

            if ((n <= 100) || (n > 100 && n < 1000 && weight < 0.8) || (n >= 1000 && n < 10000 && weight < 0.5) || (n >= 10000 && weight < 0.3)) {
                // if neighbor is not in S, add it to H
                if (dist[i] > weight && S.find(i) == S.end())
                {
                    dist[i] = weight;
                    H.insert(make_tuple(i, weight));
                }
            }
            i++;
        }
    }
    float total_weight = 0.0;
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
            // edge pruning
            if ((n <= 100) || (n > 100 && n < 1000 && randnum < 0.2) || (n >= 1000 && n < 10000 && randnum < 0.1) || (n >= 10000 && randnum < 0.01))
            {
                (*(vList + i)).push_back(make_tuple(j, randnum));
                (vList + j)->push_back(make_tuple(i, randnum));
                c++;
            }
        }
    }
    return c;
}