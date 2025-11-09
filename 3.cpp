
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <command> [args...]\n";
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // Дочерний процесс
        execvp(argv[1], &argv[1]);
        perror("error with execvp");
        return EXIT_FAILURE;
    } else {
        // Родитель ждёт дочернего процесса
        wait(NULL);
    }

    return 0;
}

