// include std library
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;


// print vector of vectors matrix
void print_matrix(vector<vector<int>> &matrix) {
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
void matrix_multiply(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C) 
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
void add_matrix(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C)
{
    int m = A.size();
    int n = B.size();
    
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}


// helper subtraction function for Strassen's
void subtract_matrix(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C)
{
    int m = A.size();
    int n = B.size();
    
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

// initialize a vector of vectors m x n matrix with random numbers, pass by reference
void init_matrix(vector<vector<int>> &matrix, int m, int n)
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
void pad_matrix(vector<vector<int>> &matrix, int rows, int cols, int dim)
{
    // pad existing rows
    for (auto &row : matrix)
    {
        for (int r = cols; r < dim; r++)
        {
            row.push_back(0);
        }
    }
    // pad by adding more rows
    for (int i = rows; i < dim; i++)
    {
        vector<int> new_row;
        for (int j = 0; j < dim; j++)
        {
            new_row.push_back(0);
        }
        matrix.push_back(new_row);
    }
}


// prune the result matrix of padded zeros
void prune_matrix(vector<vector<int>> &C, int m, int p) {
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
void strassen_matrix(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C)
{   
    int m = A.size();
    int n = B.size();
    int p = B[0].size();
    // check if padding is needed
    // (m & (m - 1)) != 0
    if (m != n || n != p || (m & (m - 1)) != 0)
    {
        // calculate target matrix dimension
        int max_dim = max({m, n, p});
        int dimension = pow(2, ceil(log2(max_dim)));
        pad_matrix(A, m, n, dimension);
        pad_matrix(B, n, p, dimension);
        pad_matrix(C, m, p, dimension);
    }

    // now we have 2^n x 2^n matrices as A, B, base case for strassen's is 2 x 2
    // base case
    int A_length = A.size();

    if (A_length == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    // initialize submatrices a through h
    int half = A_length / 2;

    // initialize submatrices a through h
    vector<vector<int>> a(half, vector<int>(half));
    vector<vector<int>> b(half, vector<int>(half));
    vector<vector<int>> c(half, vector<int>(half));
    vector<vector<int>> d(half, vector<int>(half));
    vector<vector<int>> e(half, vector<int>(half));
    vector<vector<int>> f(half, vector<int>(half));
    vector<vector<int>> g(half, vector<int>(half));
    vector<vector<int>> h(half, vector<int>(half));
    
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
    vector<vector<int>> p1(half, vector<int>(half));
    vector<vector<int>> p2(half, vector<int>(half));
    vector<vector<int>> p3(half, vector<int>(half));
    vector<vector<int>> p4(half, vector<int>(half));
    vector<vector<int>> p5(half, vector<int>(half));
    vector<vector<int>> p6(half, vector<int>(half));
    vector<vector<int>> p7(half, vector<int>(half));

    // two temp arrays
    vector<vector<int>> temp1(half, vector<int>(half));
    vector<vector<int>> temp2(half, vector<int>(half));


    // calculate P1 through P7
    // P1 = A(F - H)
    subtract_matrix(f, h, temp1);
    strassen_matrix(a, temp1, p1);

    // P2 = (A + B)H
    add_matrix(a, b, temp1);
    strassen_matrix(temp1, h, p2);

    // P3 = (C + D)E
    add_matrix(c, d, temp1);
    strassen_matrix(temp1, e, p3);

    // P4 = D(G - E)
    subtract_matrix(g, e, temp1);
    strassen_matrix(d, temp1, p4);

    // P5 = (A + D)(E + H)
    add_matrix(a, d, temp1);
    add_matrix(e, h, temp2);
    strassen_matrix(temp1, temp2, p5);

    // P6 = (B - D)(G + H)
    subtract_matrix(b, d, temp1);
    add_matrix(g, h, temp2);
    strassen_matrix(temp1, temp2, p6);

    // P7 = (C - A)(E + F)
    subtract_matrix(c, a, temp1);
    add_matrix(e, f, temp2);
    strassen_matrix(temp1, temp2, p7);

    
    // combine submatrices of C
    vector<vector<int>> C1(half, vector<int>(half)); 
    vector<vector<int>> C2(half, vector<int>(half));
    vector<vector<int>> C3(half, vector<int>(half));
    vector<vector<int>> C4(half, vector<int>(half));

    // C1 = P4 + P5 + P6 - P2
    add_matrix(p4, p5, temp1);
    subtract_matrix(p6, p2, temp2);
    add_matrix(temp1, temp2, C1);

    // C2 = P1 + P2
    add_matrix(p1, p2, C2);

    // C3 = P3 + P4
    add_matrix(p3, p4, C3);

    // C4 = P1 - p3 + p5 + p7
    subtract_matrix(p1, p3, temp1);
    add_matrix(p5, p7, temp2);
    add_matrix(temp1, temp2, C4);
    

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


// main function
int main(void)
{
    srand(static_cast<unsigned>(time(0)));

    // initialize a matrix of m x n, and another one of n x p
    int m = 5;
    int n = 5;
    int p = 5;
    vector<vector<int>> matrix(m, vector<int>(n));
    vector<vector<int>> matrix2(n, vector<int>(p));
    // vector<vector<int>> result_matrix(m, vector<int>(p));

    init_matrix(matrix, m, n);
    init_matrix(matrix2, n, p);

    // cout << "Matrix 1: " << endl;
    // print_matrix(matrix);
    // cout << "Matrix 2: " << endl;
    // print_matrix(matrix2);

    // // multiply matrices
    // matrix_multiply(matrix, matrix2, result_matrix);
    // cout << "Naive matrix multiplication:" << endl;
    // print_matrix(result_matrix);
    
    // // Strassen's matrix multiplication
    // strassen_matrix(matrix, matrix2, result_matrix);
    // prune_matrix(result_matrix, m, p);
    // cout << "Strassen's matrix multiplication:" << endl;
    // print_matrix(result_matrix);


    // experiment with different matrix sizes, and time the results
    for(int i = 2; i < 2048; i*=2) {
        vector<vector<int>> matrix(i, vector<int>(i));
        vector<vector<int>> matrix2(i, vector<int>(i));
        vector<vector<int>> result_matrix(i, vector<int>(i));

        init_matrix(matrix, i, i);
        init_matrix(matrix2, i, i);

        auto start = chrono::high_resolution_clock::now();
        matrix_multiply(matrix, matrix2, result_matrix);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Time taken for " << i << "x" << i << " matrix: " << duration.count() << " miliseconds" << endl;

        start = chrono::high_resolution_clock::now();
        strassen_matrix(matrix, matrix2, result_matrix);
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Time taken for " << i << "x" << i << " matrix using Strassen's algorithm: " << duration.count() << " miliseconds" << endl;
    }
    
    // return 0;
}
