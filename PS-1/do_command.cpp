#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>

void do_command(char** argv) {
    auto start = std::chrono::steady_clock::now();
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        execvp(argv[0], argv);
        perror("[Child] execvp failed");
        _exit(1);
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return;
        }
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

        if (WIFEXITED(status)) {
            std::cout << "[Parent] Child finished with exit code "
                      << WEXITSTATUS(status)
                      << " and took "
                      << duration.count()
                      << " seconds.\n";
        } else {
            std::cout << "[Parent] Child terminated abnormally.\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./do-command <command> [args...]\n";
        return 1;
    }
    do_command(&argv[1]);
    return 0;
}

       
