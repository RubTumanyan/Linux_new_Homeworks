#include <iostream>
#include <pthread.h>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;

vector<int> arr;
long long global_sum = 0;
pthread_mutex_t sum_mutex;

struct ThreadData {
    int start;
    int end;
};

void* partial_sum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long local_sum = 0;
    for (int i = data->start; i < data->end; i++)
        local_sum += arr[i];
    pthread_mutex_lock(&sum_mutex);
    global_sum += local_sum;
    pthread_mutex_unlock(&sum_mutex);
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./array-summary <N> <M>\n";
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    if (N <= 0 || M <= 0) {
        cerr << "N and M must be > 0\n";
        return 1;
    }

    arr.resize(N);
    for (int i = 0; i < N; i++)
        arr[i] = rand() % 100;

    auto start_single = chrono::high_resolution_clock::now();
    long long single_sum = 0;
    for (int x : arr)
        single_sum += x;
    auto end_single = chrono::high_resolution_clock::now();

    auto duration_single = chrono::duration_cast<chrono::milliseconds>(end_single - start_single).count();
    cout << "Time spent without threads: " << duration_single << " ms\n";

    pthread_mutex_init(&sum_mutex, nullptr);
    global_sum = 0;

    vector<pthread_t> threads(M);
    vector<ThreadData> threadData(M);
    int chunkSize = N / M;

    auto start_multi = chrono::high_resolution_clock::now();

    for (int i = 0; i < M; i++) {
        threadData[i].start = i * chunkSize;
        threadData[i].end = (i == M - 1) ? N : (i + 1) * chunkSize;
        pthread_create(&threads[i], nullptr, partial_sum, &threadData[i]);
    }

    for (int i = 0; i < M; i++)
        pthread_join(threads[i], nullptr);

    auto end_multi = chrono::high_resolution_clock::now();
    auto duration_multi = chrono::duration_cast<chrono::milliseconds>(end_multi - start_multi).count();
    pthread_mutex_destroy(&sum_mutex);

    cout << "Threads spent time : " << duration_multi << " ms\n";

    if (single_sum == global_sum)
        cout << " match. Result = " << global_sum << "\n";
    else
        cout << " do not match!\n";

    return 0;
}

