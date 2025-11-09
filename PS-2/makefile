CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
TARGET = interactive-shell
SRCS = interactive-shell.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o *.log
