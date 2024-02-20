#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

const int N = 250; // Change N to the desired size of the matrices

void initializeMatrix(int matrix[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 10; // Generate random values between 0 and 9
        }
    }
}

void multiplyMatrices(int A[N][N], int B[N][N], int C[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void writeMatrixToFile(int matrix[N][N], const char* fileName) {
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                outFile << matrix[i][j] << " ";
            }
            outFile << endl;
        }
        outFile.close();
        cout << "Output written to " << fileName << endl;
    } else {
        cerr << "Unable to open file: " << fileName << endl;
    }
}

int main() {
    int A[N][N], B[N][N], C[N][N];

    // Initialize random seed
    srand(time(0));

    // Initialize matrices A and B with random values
    initializeMatrix(A);
    initializeMatrix(B);

    // Measure execution time
    auto start = high_resolution_clock::now();

    // Perform matrix multiplication
    multiplyMatrices(A, B, C);

    // Measure execution time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Print execution time
    cout << "Execution Time: " << duration.count() << " microseconds" << endl;

    // Write the result matrix C to a file
    writeMatrixToFile(C, "output.txt");

    return 0;
}
