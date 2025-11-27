#include <iostream>
#include <csignal>
#include <unistd.h>
#include <pwd.h>
#include <cstring>
#include <ucontext.h>
#include <stdexcept>

void sigusr1_handler(int signo, siginfo_t *info, void *context) {
    try {
        if (signo != SIGUSR1) return;

        pid_t sender_pid = info->si_pid;
        uid_t sender_uid = info->si_uid;

        struct passwd *pw = getpwuid(sender_uid);
        const char *username = pw ? pw->pw_name : "unknown";

        ucontext_t *uc = (ucontext_t*)context;

#if defined(__x86_64__)
        greg_t rip = uc->uc_mcontext.gregs[REG_RIP];
        greg_t rax = uc->uc_mcontext.gregs[REG_RAX];
        greg_t rbx = uc->uc_mcontext.gregs[REG_RBX];
        std::cout << "Received SIGUSR1 from PID " << sender_pid
                  << " executed by UID " << sender_uid
                  << " (" << username << ").\n"
                  << "State of the context RIP = " << rip
                  << ", RAX = " << rax
                  << ", RBX = " << rbx << "\n";
#elif defined(__i386__)
        greg_t eip = uc->uc_mcontext.gregs[REG_EIP];
        greg_t eax = uc->uc_mcontext.gregs[REG_EAX];
        greg_t ebx = uc->uc_mcontext.gregs[REG_EBX];
        std::cout << "Received SIGUSR1 from PID " << sender_pid
                  << " executed by UID " << sender_uid
                  << " (" << username << ").\n"
                  << "State of the context: EIP = " << eip
                  << ", EAX = " << eax
                  << ", EBX = " << ebx << "\n";
#else
        std::cout << "Unsupported architecture for register display.\n";
#endif
    } catch (const std::exception &e) {
        std::cerr << "Exception in signal handler: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in signal handler" << std::endl;
    }
}

int main() {
    pid_t pid = getpid();
    std::cout << "PID of this process: " << pid << std::endl;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = sigusr1_handler;
    sa.sa_flags = SA_SIGINFO | SA_NODEFER; 
    sigemptyset(&sa.sa_mask);   
    sigaddset(&sa.sa_mask, SIGINT); 

    if (sigaction(SIGUSR1, &sa, nullptr) == -1) {
        perror("sigaction");
        return 1;
    }

    while (true) {
        sleep(10);
    }

    return 0;
}

