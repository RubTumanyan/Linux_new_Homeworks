#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>

static void split_tokens(const std::string &line, std::vector<std::string> &out) {
    std::istringstream iss(line);
    std::string tok;
    while (iss >> tok) out.push_back(tok);
}

int main() {
    std::string line;
    while (true) {
        std::cout << " "<< std::flush;
        if (!std::getline(std::cin, line)) {
            std::cout << "\n";
            break;
        }
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        size_t end = line.find_last_not_of(" \t\r\n");
        std::string cmdline = line.substr(start, end - start + 1);
        if (cmdline.empty()) continue;

        std::vector<std::string> tokens;
        split_tokens(cmdline, tokens);
        if (tokens.empty()) continue;
        if (tokens[0] == "exit") return 0;

        bool silent = false;
        if (tokens[0] == "silent") {
            silent = true;
            tokens.erase(tokens.begin());
            if (tokens.empty()) continue;
        }

        std::vector<char*> argv;
        for (auto &s : tokens) argv.push_back(const_cast<char*>(s.c_str()));
        argv.push_back(nullptr);

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != nullptr) {
                const char *oldpath = getenv("PATH");
                std::string newpath = oldpath ? std::string(cwd) + ":" + oldpath : std::string(cwd);
                setenv("PATH", newpath.c_str(), 1);
            }

            if (silent) {
                pid_t mypid = getpid();
                char fname[64];
                snprintf(fname, sizeof(fname), "%d.log", (int)mypid);
                int fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd != -1) {
                    dup2(fd, STDOUT_FILENO);
                    dup2(fd, STDERR_FILENO);
                    if (fd > 2) close(fd);
                }
            }
            execvp(argv[0], argv.data());
            perror("execvp");
            _exit(127);
        } else {
            int status = 0;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                std::cout << "Command exited" << WEXITSTATUS(status) << "\n";
            else if (WIFSIGNALED(status))
                std::cout << "signal " << WTERMSIG(status) << "\n";
            else
                std::cout << "abnormally\n";
        }
    }
    return 0;
}
