CXX = g++
CXXFLAGS = -Wall -O2
TARGET = do_command

all:$(TARGET)

$(TARGET): do_command.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) do_command.cpp

clean:
	rm -f $(TARGET)
