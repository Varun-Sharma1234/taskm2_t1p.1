#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace chrono;

const int N = 250; // Change N to the desired size of the matrices
const int NUM_THREADS = 4; // Number of threads

int A[N][N], B[N][N], C[N][N];
pthread_t threads[NUM_THREADS];

struct ThreadData {
    int startRow;
    int endRow;
};

void initializeMatrix(int matrix[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 10; // Generate random values between 0 and 9
        }
    }
}

void* multiplyRows(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    for (int i = data->startRow; i <= data->endRow; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

void writeMatrixToFile(const char* fileName) {
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                outFile << C[i][j] << " ";
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
    // Initialize random seed
    srand(time(0));

    // Initialize matrices A and B with random values
    initializeMatrix(A);
    initializeMatrix(B);

    // Measure execution time
    auto start = high_resolution_clock::now();

    // Create threads and distribute rows among them
    int rowsPerThread = N / NUM_THREADS;
    int remainingRows = N % NUM_THREADS;
    ThreadData threadData[NUM_THREADS];

    int startRow = 0;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadData[i].startRow = startRow;
        threadData[i].endRow = startRow + rowsPerThread - 1 + (i < remainingRows ? 1 : 0);
        startRow = threadData[i].endRow + 1;

        pthread_create(&threads[i], NULL, multiplyRows, &threadData[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Measure execution time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // Print execution time
    cout << "Execution Time: " << duration.count() << " microseconds" << endl;

    // Write the result matrix C to a file
    writeMatrixToFile("output.txt");

    return 0;
}
