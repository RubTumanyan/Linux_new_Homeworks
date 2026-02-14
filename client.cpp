#include "Socket.hpp"
#include <iostream>

using namespace SimpleNet;

int main() {
    Socket client;
    client.connect("127.0.0.1", 8080);

    client.send("Hello server!");

    auto data = client.receive();
    std::cout << "Server answered: " 
              << std::string(data.begin(), data.end()) 
              << std::endl;
}
