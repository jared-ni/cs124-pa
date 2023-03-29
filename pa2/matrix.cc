// include std library
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>

using namespace std;

int crosspoint = 1;

// print vector of vectors matrix
void print_matrix(vector<vector<int> > &matrix) {
    for (auto &row : matrix) {
        for (auto &col : row) {
            cout << col << " ";
        }
        cout << endl;
    }
    cout << endl;
}


// brute force matrix multiplication in O(n^3) that works on any dimension, passing vectors by reference
// A is m x n, B is n x p, C is m x p
void matrix_multiply(vector<vector<int> > &A, vector<vector<int> > &B, vector<vector<int> > &C) 
{
    int m = A.size();
    int n = B.size();
    int p = B[0].size();

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            for (int k = 0; k < n; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// helper addition function for Strassen's
void add_m(vector<vector<int> > &A, vector<vector<int> > &B, vector<vector<int> > &C, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}


// helper subtraction function for Strassen's
void subtract_m(vector<vector<int> > &A, vector<vector<int> > &B, vector<vector<int> > &C, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}


// initialize a vector of vectors m x n matrix with random numbers, pass by reference
void init_matrix(vector<vector<int> > &matrix, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = (rand() % 4) - 2;
        }
    }
}

// pad matrix dimensions to be square and a power of 2
void pad_matrix(vector<vector<int> > &matrix, int dim)
{
    // add rows
    for (int i = matrix.size(); i < dim; i++) {
        matrix.push_back(vector<int>(dim, 0));
    }
    // resize rows and columns with 0s
    for (auto &row : matrix) {
        row.resize(dim);
    }
}


// prune the result matrix of padded zeros
void prune_matrix(vector<vector<int> > &C, int m, int p) {
    int C_length = C.size();
    
    // remove rows
    for (int i = C_length - 1; i >= m; i--) {
        C.pop_back();
    }

    // remove extra zeros per row
    for (auto &row : C) {
        for (int j = C_length - 1; j >= p; j--) {
            row.pop_back();
        }
    }
}


// Strassen's matrix multiplication in O(n^2.81). A * B = C
void strassen_matrix(vector<vector<int> > &A, vector<vector<int> > &B, vector<vector<int> > &C, int size)
{   
    // now we have 2^n x 2^n matrices as A, B, base case for strassen's is 2 x 2
    // base case
    if (size == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }
    else if (size < crosspoint) {
        matrix_multiply(A, B, C);
        return;
    }
    else if (size % 2 != 0) {
        size += 1;
        pad_matrix(A, size);
        pad_matrix(B, size);
        pad_matrix(C, size);
    }

    // initialize submatrices a through h
    int half = size / 2;
    
    vector<int> row(half, 0);
    // initialize submatrices a through h
    vector<vector<int> > a(half, row);
    vector<vector<int> > b(half, row);
    vector<vector<int> > c(half, row);
    vector<vector<int> > d(half, row);
    vector<vector<int> > e(half, row);
    vector<vector<int> > f(half, row);
    vector<vector<int> > g(half, row);
    vector<vector<int> > h(half, row);
    
    for (int i = 0; i < half; i++)
    {
        for (int j = 0; j < half; j++)
        {
            a[i][j] = A[i][j];
            e[i][j] = B[i][j];

            b[i][j] = A[i][j + half];
            f[i][j] = B[i][j + half];

            c[i][j] = A[i + half][j];
            g[i][j] = B[i + half][j];

            d[i][j] = A[i + half][j + half];
            h[i][j] = B[i + half][j + half];
        }
    }
    
    // initiate P1 through P7
    vector<vector<int> > p1(half, row);
    vector<vector<int> > p2(half, row);
    vector<vector<int> > p3(half, row);
    vector<vector<int> > p4(half, row);
    vector<vector<int> > p5(half, row);
    vector<vector<int> > p6(half, row);
    vector<vector<int> > p7(half, row);

    // two temp arrays
    vector<vector<int> > temp1(half, row);
    vector<vector<int> > temp2(half, row);


    // calculate P1 through P7
    // P1 = A(F - H)
    subtract_m(f, h, temp1, half);
    strassen_matrix(a, temp1, p1, half);

    // P2 = (A + B)H
    add_m(a, b, temp1, half);
    strassen_matrix(temp1, h, p2, half);

    // P3 = (C + D)E
    add_m(c, d, temp1, half);
    strassen_matrix(temp1, e, p3, half);

    // P4 = D(G - E)
    subtract_m(g, e, temp1, half);
    strassen_matrix(d, temp1, p4, half);

    // P5 = (A + D)(E + H)
    add_m(a, d, temp1, half);
    add_m(e, h, temp2, half);
    strassen_matrix(temp1, temp2, p5, half);

    // P6 = (B - D)(G + H)
    subtract_m(b, d, temp1, half);
    add_m(g, h, temp2, half);
    strassen_matrix(temp1, temp2, p6, half);

    // P7 = (C - A)(E + F)
    subtract_m(c, a, temp1, half);
    add_m(e, f, temp2, half);
    strassen_matrix(temp1, temp2, p7, half);

    
    // combine submatrices of C
    vector<vector<int> > C1(half, row); 
    vector<vector<int> > C2(half, row);
    vector<vector<int> > C3(half, row);
    vector<vector<int> > C4(half, row);

    // C1 = P4 + P5 + P6 - P2
    add_m(p4, p5, temp1, half);
    subtract_m(p6, p2, temp2, half);
    add_m(temp1, temp2, C1, half);

    // C2 = P1 + P2
    add_m(p1, p2, C2, half);

    // C3 = P3 + P4
    add_m(p3, p4, C3, half);

    // C4 = P1 - p3 + p5 + p7
    subtract_m(p1, p3, temp1, half);
    add_m(p5, p7, temp2, half);
    add_m(temp1, temp2, C4, half);
    

    // combine submatrices of C into single matrix 
    for (int i = 0; i < half; i++)
    {
        for (int j = 0; j < half; j++)
        {
            C[i][j] = C1[i][j];
            C[i][j + half] = C2[i][j];
            C[i + half][j] = C3[i][j];
            C[i + half][j + half] = C4[i][j];
        }
    }
}

// initialize random graph including edges with probability p = 0.01 - 0.05
vector<vector<int> > initialize_graph(int dim, float p)
{
    vector<vector<int> > G(dim, vector<int>(dim));
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            G[i][j] = 0;
            bool edge = (rand() % 100) < (p * 100);
            if (edge == true && i != j) {
                G[i][j] = 1;
            }
        }
    }
    return G;
}

// calculate number of triangles in a graph G
int calculate_triangles(vector<vector<int>> &G)
{
    int dim = G.size();
    vector<vector<int> > G2(dim, vector<int>(dim));
    strassen_matrix(G, G, G2, dim);

    vector<vector<int> > G3(dim, vector<int>(dim));
    strassen_matrix(G2, G, G3, dim);

    int num_triangles = 0;
    for (int i = 0; i < dim; i++) {
        num_triangles += G3[i][i];
    }

    return num_triangles / 6;
}


// part 2 experiment time function
void part2loop(vector<vector<int> > &matrix, vector<vector<int> > &matrix2, 
               vector<vector<int> > &result_matrix, fstream &fout) {
    for(int i = 2; i < 3; i*=2) {
        pad_matrix(matrix, i);
        pad_matrix(matrix2, i);
        pad_matrix(result_matrix, i);

        init_matrix(matrix, i, i);
        init_matrix(matrix2, i, i);

        // repeat 5 trials for each matrix size
        for (int j = 0; j < 1; j++) {
            std::__1::chrono::steady_clock::time_point start = chrono::high_resolution_clock::now();
            matrix_multiply(matrix, matrix2, result_matrix);
            cout << "Brute force: " << endl;
            print_matrix(result_matrix);

            std::__1::chrono::steady_clock::time_point end = chrono::high_resolution_clock::now();
            std::__1::chrono::milliseconds duration = chrono::duration_cast<chrono::milliseconds>(end - start);
            // cout << "Time taken for " << i << "x" << i << " matrix: " << duration.count() << " miliseconds" << endl;
            fout << crosspoint << "," << i<<"x"<<i<< "," << duration.count();

            start = chrono::high_resolution_clock::now();
            strassen_matrix(matrix, matrix2, result_matrix, i);
            prune_matrix(result_matrix, i, i);

            cout << "Strassen's: " << endl;
            print_matrix(result_matrix);
            end = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::milliseconds>(end - start);
            fout << "," << duration.count() << endl;
            // cout << "Time taken for " << i << "x" << i << " matrix using Strassen's algorithm: " << duration.count() << " miliseconds" << endl;
        }
    }
}


void part3loop()
{
    for (float i = 0.01; i < 0.06; i += 0.01)
    {
        vector<vector<int>> triangle_graph = initialize_graph(1024, i);
        int num_triangles = calculate_triangles(triangle_graph);
        cout << num_triangles << endl;
    }  
}


// main function
int main(int argc, char *argv[])
{
    srand(static_cast<unsigned>(time(0)));

    crosspoint = 250;

    // initialize a matrix of m x n, and another one of n x p
    int m = 2;
    int n = 4;
    int p = 2;
    
    vector<vector<int> > matrix(m, vector<int>(n));
    vector<vector<int> > matrix2(n, vector<int>(p));
    vector<vector<int> > result_matrix(m, vector<int>(p));

    // Part 1: Multiply two matrices
    init_matrix(matrix, m, n);
    print_matrix(matrix);

    init_matrix(matrix2, n, p);
    print_matrix(matrix2);

    matrix_multiply(matrix, matrix2, result_matrix);
    cout << "Brute force: " << endl;
    print_matrix(result_matrix);

    strassen_matrix(matrix, matrix2, result_matrix, m);
    prune_matrix(result_matrix, m, p);
    cout << "Strassen's: " << endl;
    print_matrix(result_matrix);

        
    // Part 2: Experimentally determine crossover point
    // fstream fout;
    // fout.open("part2-1025-250s.csv", ios::out);
    // fout<<"Crossover point"<<","<<"Dimension"<<","<<"Brute Force (milisec)"<<","<<"Strassen's (milisec)"<<endl;
    // while(crosspoint < 300) {
    //     // cout << "Crossover point: " << crosspoint << endl;
    //     vector<vector<int> > matrix;
    //     vector<vector<int> > matrix2;
    //     vector<vector<int> > result_matrix;
    //     part2loop(matrix, matrix2, result_matrix, fout);
    //     crosspoint += 1;
    // }
    // fout.close();

    // Part 3: Calculate number of triangles for p = 0.01 through 0.05
    // for (int i = 0; i < 5; i++)
    // {
    //     part3loop();
    // }
    
    return 0;
}
