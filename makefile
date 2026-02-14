CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall

SERVER_SRC = Socket.cpp ThreadPool.cpp TcpServer.cpp main.cpp
SERVER_BIN = server

CLIENT_SRC = Socket.cpp client.cpp
CLIENT_BIN = client

all: server client

server: $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_BIN)

client: $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(CLIENT_SRC) -o $(CLIENT_BIN)


clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)
