CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
TARGET = signal-echo

all: $(TARGET)

$(TARGET): signal-echo.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) signal-echo.cpp

clean:
	rm -f $(TARGET)
