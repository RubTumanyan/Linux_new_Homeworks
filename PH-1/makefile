CXX = g++
CXXFLAGS = -O2 -pthread -std=c++17
TARGET = array-summary

all: $(TARGET)

$(TARGET): sum.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) sum.cpp

clean:
	rm -f $(TARGET)

