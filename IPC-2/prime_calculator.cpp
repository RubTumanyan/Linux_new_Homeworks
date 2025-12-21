#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>

using namespace std;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0)
            return false;
    return true;
}

int nthPrime(int m) {
    int count = 0;
    int number = 1;
    while (count < m) {
        number++;
        if (isPrime(number))
            count++;
    }
    return number;
}

int main() {
    int pipe_pc[2];
    int pipe_cp[2];

    if (pipe(pipe_pc) == -1 || pipe(pipe_cp) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(pipe_pc[1]);
        close(pipe_cp[0]);
        while (true) {
            int m;
            ssize_t bytes = read(pipe_pc[0], &m, sizeof(m));
            if (bytes <= 0 || m == -1) break;
            cout << "[Child] Calculating " << m << "-th prime number..." << endl;
            int result = nthPrime(m);
            cout << "[Child] Sending calculation result of prime(" << m << ")..." << endl;
            write(pipe_cp[1], &result, sizeof(result));
        }
        close(pipe_pc[0]);
        close(pipe_cp[1]);
        exit(0);
    } else {
        close(pipe_pc[0]);
        close(pipe_cp[1]);
        while (true) {
            cout << "[Parent] Please enter the number: ";
            string input;
            cin >> input;
            if (input == "exit") {
                int stop = -1;
                write(pipe_pc[1], &stop, sizeof(stop));
                break;
            }
            int m = stoi(input);
            cout << "[Parent] Sending " << m << " to the child process..." << endl;
            write(pipe_pc[1], &m, sizeof(m));
            cout << "[Parent] response from the child process..." << endl;
            int result;
            read(pipe_cp[0], &result, sizeof(result));
            cout << "[Parent] result of prime(" << m << ") = " << result << "..." << endl;
        }
        close(pipe_pc[1]);
        close(pipe_cp[0]);
        wait(NULL);
    }
    return 0;
}

