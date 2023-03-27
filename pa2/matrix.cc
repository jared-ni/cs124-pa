// include std library
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>

using namespace std;

// brute force matrix multiplication in O(n^3) that works on any dimension, passing vectors by reference
// A is m x n, B is n x p, C is m x p
vector<vector<int>> matrix_multiply(vector<vector<int>> &A, vector<vector<int>> &B)
{
    int m = A.size();
    int n = B.size();
    int p = B[0].size();
    
    vector<vector<int>> C(m, vector<int>(p));
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
    return C;
}

// helper addition function for Strassen's
vector<vector<int>> add_matrix(vector<vector<int>> &A, vector<vector<int>> &B)
{
    int m = A.size();
    int n = B.size();
    
    vector<vector<int>> C(m, vector<int>(n));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

// helper subtraction function for Strassen's
vector<vector<int>> subtract_matrix(vector<vector<int>> &A, vector<vector<int>> &B)
{
    int m = A.size();
    int n = B.size();

    vector<vector<int>> C(m, vector<int>(n));

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
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
vector<vector<int>> strassen_matrix(vector<vector<int>> &A, vector<vector<int>> &B)
{   
    int m = A.size();
    int n = B.size();
    int p = B[0].size();
    // check if padding is needed
    if (m != n || n != p || m % 2 != 0)
    {
        // calculate target matrix dimension
        int max_dim = max({m, n, p});
        int dimension = pow(2, ceil(log2(max_dim)));
        pad_matrix(A, m, n, dimension);
        pad_matrix(B, n, p, dimension);
    }

    // now we have 2^n x 2^n matrices as A, B, base case for strassen's is 2 x 2
    // base case
    int A_length = A.size();
    vector<vector<int>> C(A_length, vector<int>(A_length));
    
    if (A_length == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return C;
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

    // calculate P1 through P7
    vector<vector<int>> f_minus_h = subtract_matrix(f, h);
    vector<vector<int>> a_plus_b = add_matrix(a, b);
    vector<vector<int>> c_plus_d = add_matrix(c, d);
    vector<vector<int>> g_minus_e = subtract_matrix(g, e);
    vector<vector<int>> a_plus_d = add_matrix(a, d);
    vector<vector<int>> e_plus_h = add_matrix(e, h);
    vector<vector<int>> b_minus_d = subtract_matrix(b, d);
    vector<vector<int>> g_plus_h = add_matrix(g, h);
    vector<vector<int>> c_minus_a = subtract_matrix(c, a);
    vector<vector<int>> e_plus_f = add_matrix(e, f);
    
    vector<vector<int>> p1 = strassen_matrix(a, f_minus_h);
    vector<vector<int>> p2 = strassen_matrix(a_plus_b, h);
    vector<vector<int>> p3 = strassen_matrix(c_plus_d, e);
    vector<vector<int>> p4 = strassen_matrix(d, g_minus_e);
    vector<vector<int>> p5 = strassen_matrix(a_plus_d, e_plus_h);
    vector<vector<int>> p6 = strassen_matrix(b_minus_d, g_plus_h);
    vector<vector<int>> p7 = strassen_matrix(c_minus_a, e_plus_f);

    
    // combine submatrices of C
    vector<vector<int>> C11 = add_matrix(p5, p4);
    vector<vector<int>> C12 = subtract_matrix(C11, p2);
    vector<vector<int>> C1 = add_matrix(C12, p6);

    vector<vector<int>> C2 = add_matrix(p1, p2);

    vector<vector<int>> C3 = add_matrix(p3, p4);

    vector<vector<int>> C41 = subtract_matrix(p1, p3);
    vector<vector<int>> C42 = add_matrix(C41, p5);
    vector<vector<int>> C4 = add_matrix(C42, p7);

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
    
    return C;
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

    srand(static_cast<unsigned>(time(0)));

    // initialize a matrix of m x n
    init_matrix(matrix, 2, 4);
    cout << "Matrix 1:" << endl;
    print_matrix(matrix);
    init_matrix(matrix2, 4, 2);
    cout << "Matrix 2:" << endl;
    print_matrix(matrix2);

    vector<vector<int>> result_matrix = matrix_multiply(matrix, matrix2);
    cout << "Correct Result Matrix:" << endl;
    print_matrix(result_matrix);

    // Strassen's matrix multiplication
    vector<vector<int>> strassen_result = strassen_matrix(matrix, matrix2);

    cout << "Padded matrix 1:" << endl;
    print_matrix(matrix);

    cout << "Padded matrix 2:" << endl;
    print_matrix(matrix2);

    cout << "Strassen's matrix multiplication:" << endl;
    print_matrix(strassen_result);

    return 0;
}
