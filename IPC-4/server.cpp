#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

struct Client {
    int sock;
    string name;
};

vector<Client> clients;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcastMessage(const string &msg, int senderSock) {
    pthread_mutex_lock(&clients_mutex);
    for (auto &c : clients) {
        if (c.sock != senderSock) {
            send(c.sock, msg.c_str(), msg.size(), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void sendUserList(int sock) {
    pthread_mutex_lock(&clients_mutex);
    string list = "Connected users:\n";
    for (auto &c : clients) list += c.name + "\n";
    pthread_mutex_unlock(&clients_mutex);
    send(sock, list.c_str(), list.size(), 0);
}

void *handleClient(void *arg) {
    int sock = *(int *)arg;
    char buffer[1024];
    
    int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) { close(sock); pthread_exit(nullptr); }
    buffer[bytes] = '\0';
    string name = buffer;

    pthread_mutex_lock(&clients_mutex);
    clients.push_back({sock, name});
    pthread_mutex_unlock(&clients_mutex);

    string joinMsg = name + " joined the chat.\n";
    broadcastMessage(joinMsg, sock);

    while (true) {
        bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        string msg = buffer;

        if (msg == "/exit") break;
        if (msg == "/list") {
            sendUserList(sock);
            continue;
        }

        string fullMsg = name + ": " + msg + "\n";
        broadcastMessage(fullMsg, sock);
    }

    pthread_mutex_lock(&clients_mutex);
    clients.erase(remove_if(clients.begin(), clients.end(),
                            [sock](Client &c){ return c.sock == sock; }),
                  clients.end());
    pthread_mutex_unlock(&clients_mutex);

    string leaveMsg = name + " left the chat.\n";
    broadcastMessage(leaveMsg, sock);

    close(sock);
    pthread_exit(nullptr);
}

int main() {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) { perror("socket"); return 1; }

    int opt = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);

    if (bind(serverSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind"); return 1;
    }

    if (listen(serverSock, 10) < 0) {
        perror("listen"); return 1;
    }

    cout << "Server started on port 12345..." << endl;

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);
        int clientSock = accept(serverSock, (sockaddr*)&clientAddr, &addrLen);
        if (clientSock < 0) continue;

        pthread_t tid;
        int *pclient = new int;
        *pclient = clientSock;
        pthread_create(&tid, nullptr, handleClient, pclient);
        pthread_detach(tid);
    }

    close(serverSock);
    return 0;
}

