// include std library
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>

using namespace std;

// brute force matrix multiplication in O(n^3) that works on any dimension, passing vectors by reference
// A is m x n, B is n x p, C is m x p
void matrix_multiply(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C, int m, int n, int p)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < n; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// helper addition function for Strassen's
void add_matrix(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

// helper subtraction function for Strassen's
void subtract_matrix(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C, int m, int n)
{
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
        vector<int> row;
        for (int j = 0; j < n; j++)
        {
            row.push_back((rand() % 20) - 10);
        }
        matrix.push_back(row);
    }
}

// initialize result matrix with 0s
void init_result_matrix(vector<vector<int>> &matrix, int m, int p)
{
    for (int i = 0; i < m; i++)
    {
        vector<int> row;
        for (int j = 0; j < p; j++)
        {
            row.push_back(0);
        }
        matrix.push_back(row);
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

// Strassen's matrix multiplication in O(n^2.81). A * B = C
void strassen_matrix(vector<vector<int>> &A, vector<vector<int>> &B, vector<vector<int>> &C, int m, int n, int p)
{
    if (m != n || n != p || m % 2 != 0 || n % 2 != 0)
    {
        // calculate target matrix dimension
        int max_dim = max({m, n, p});
        int dimension = pow(2, ceil(log2(max_dim)));
        pad_matrix(A, m, n, dimension);
        pad_matrix(B, n, p, dimension);
    }
}

// print vector of vectors matrix
void print_matrix(vector<vector<int>> &matrix)
{
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// main function
int main(void)
{
    vector<vector<int>> matrix;
    vector<vector<int>> matrix2;
    vector<vector<int>> result_matrix;

    srand(static_cast<unsigned>(time(0)));

    // initialize a matrix of m x n
    init_matrix(matrix, 2, 4);

    print_matrix(matrix);

    init_matrix(matrix2, 4, 2);

    print_matrix(matrix2);

    init_result_matrix(result_matrix, 2, 2);

    print_matrix(result_matrix);

    matrix_multiply(matrix, matrix2, result_matrix, 2, 4, 2);

    print_matrix(result_matrix);

    strassen_matrix(matrix, matrix2, result_matrix, 2, 4, 2);

    print_matrix(matrix);

    print_matrix(matrix2);

    return 0;
}
