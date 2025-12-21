#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

void receiveMessages(int sock) {
    char buffer[1024];
    while (true) {
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        cout << buffer;
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect"); return 1;
    }

    cout << "Enter your name: ";
    string name;
    getline(cin, name);
    send(sock, name.c_str(), name.size(), 0);

    thread recvThread(receiveMessages, sock);

    while (true) {
        string msg;
        getline(cin, msg);
        send(sock, msg.c_str(), msg.size(), 0);
        if (msg == "/exit") break;
    }

    close(sock);
    recvThread.join();
    return 0;
}

