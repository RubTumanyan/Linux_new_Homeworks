CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17
LDFLAGS = -pthread

all: first second

first: first.cpp shared_array.cpp
	$(CXX) $(CXXFLAGS) $^ -o first $(LDFLAGS)

second: second.cpp shared_array.cpp
	$(CXX) $(CXXFLAGS) $^ -o second $(LDFLAGS)

clean:
	rm -f first second

