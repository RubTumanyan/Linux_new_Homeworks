#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>

using namespace std;

void thread_sum(const vector<int>& arr, size_t start, size_t end, long long &result) {
    long long sum = 0;
    for(size_t i = start; i < end; ++i)
        sum += arr[i];
    result = sum;
}

int main(int argc, char* argv[]) {
    if(argc != 3) {
        cerr << "Usage: ./array-summary <N> <M>\n";
        return 1;
    }

    size_t N = stoull(argv[1]);
    size_t M = stoull(argv[2]);

    if(N <= 1000000 || M < 1) {
        cerr << "N must be > 1,000,000 and M >= 1\n";
        return 1;
    }

    // Инициализация массива случайными числами
    vector<int> arr(N);
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(1, 100);

    for(size_t i = 0; i < N; ++i)
        arr[i] = dist(rng);

    // Суммирование без потоков
    auto start = chrono::high_resolution_clock::now();
    long long sum_single = 0;
    for(size_t i = 0; i < N; ++i)
        sum_single += arr[i];
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_single = end - start;
    cout << "Time spent without threads: " << duration_single.count() << " s" << endl;

    // Суммирование с потоками
    vector<thread> threads;
    vector<long long> results(M, 0);

    size_t block_size = N / M;
    start = chrono::high_resolution_clock::now();

    for(size_t i = 0; i < M; ++i) {
        size_t begin = i * block_size;
        size_t finish = (i == M-1) ? N : begin + block_size;
        threads.emplace_back(thread_sum, cref(arr), begin, finish, ref(results[i]));
    }

    for(auto &t : threads)
        t.join();

    long long sum_multi = 0;
    for(size_t i = 0; i < M; ++i)
        sum_multi += results[i];

    end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_multi = end - start;

    cout << "Time spent with " << M << " threads: " << duration_multi.count() << " s" << endl;

    // Проверка корректности
    if(sum_single != sum_multi)
        cout << "Warning: sums do not match!" << endl;

    return 0;
}

