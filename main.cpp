#include "TcpServer.hpp"
#include <iostream>

using namespace SimpleNet;

int main() {
    TcpServer server(8080, 4); // порт 8080, 4 потока

    server.run([](Socket client) {
        try {
            while (true) {
                auto data = client.receive();
                if (data.empty())
                    break;

                client.send(std::string_view(data.data(), data.size()));
            }
        } catch (...) {
            std::cout << "Client disconnected\n";
        }
    });
}
