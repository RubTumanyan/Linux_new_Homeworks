CXX = g++
CXXFLAGS = -std=c++17 -O2
LDFLAGS =

all: copy

copy: copy.cpp
	$(CXX) $(CXXFLAGS) -o $@ copy.cpp $(LDFLAGS)

clean:
	rm -f copy

