#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>

void execute_command(const std::vector<std::string>& args, bool silent=false, const std::string& outfile="", bool append=false) {
    pid_t pid = fork();
    if (pid < 0) return;
    else if (pid == 0) {
        const char* old_path = getenv("PATH");
        std::string new_path = ".";
        if (old_path) new_path += ":" + std::string(old_path);
        setenv("PATH", new_path.c_str(), 1);

        int fd = -1;
        if (silent) {
            std::string log_file = std::to_string(getpid()) + ".log";
            fd = open(log_file.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd >= 0) { dup2(fd, STDOUT_FILENO); dup2(fd, STDERR_FILENO); close(fd); }
        } else if (!outfile.empty()) {
            int flags = O_CREAT | O_WRONLY | (append ? O_APPEND : O_TRUNC);
            fd = open(outfile.c_str(), flags, 0644);
            if (fd >= 0) { dup2(fd, STDOUT_FILENO); close(fd); }
        }

        std::vector<char*> cargs;
        for (auto &s : args) cargs.push_back(const_cast<char*>(s.c_str()));
        cargs.push_back(nullptr);
        execvp(cargs[0], cargs.data());
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

std::vector<std::string> split(const std::string& str, char delim=' ') {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string temp;
    while (std::getline(ss, temp, delim)) if (!temp.empty()) tokens.push_back(temp);
    return tokens;
}

int main() {
    std::string line;
    while (true) {
        std::cout << "shell> ";
        std::getline(std::cin, line);
        if (line.empty()) continue;
        if (line == "exit") break;

        std::vector<std::string> semi_cmds = split(line, ';');
        for (auto &cmd_str : semi_cmds) {
            std::vector<std::string> tokens = split(cmd_str);
            bool run_next = true;
            for (size_t i = 0; i < tokens.size();) {
                if (!run_next) break;
                bool silent = false;
                std::string outfile;
                bool append = false;
                std::vector<std::string> args;

                if (tokens[i] == "silent") { silent = true; ++i; }

                while (i < tokens.size() && tokens[i] != "&&" && tokens[i] != "||") {
                    if (tokens[i] == ">") { ++i; if (i < tokens.size()) { outfile = tokens[i]; append = false; ++i; } }
                    else if (tokens[i] == ">>") { ++i; if (i < tokens.size()) { outfile = tokens[i]; append = true; ++i; } }
                    else { args.push_back(tokens[i]); ++i; }
                }

                if (!args.empty()) execute_command(args, silent, outfile, append);

                if (i < tokens.size()) {
                    std::string op = tokens[i]; ++i;
                    int last_status = 0; 
                    if (op == "&&") run_next = (last_status == 0);
                    else if (op == "||") run_next = (last_status != 0);
                }
            }
        }
    }
    return 0;
}
