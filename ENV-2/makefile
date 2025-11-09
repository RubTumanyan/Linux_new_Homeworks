
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
SRC_DIRS = . Complex Sorting
OBJ_DIR = obj
TARGET = env2


SRCS = main.cpp Complex/Complex.cpp Sorting/Sorting.cpp
OBJS = $(SRCS:.cpp=.o)


all: $(TARGET)


$(TARGET): $(OBJS)
        $(CXX) $(CXXFLAGS) -o $@ $^


%.o: %.cpp
        $(CXX) $(CXXFLAGS) -c $< -o $@


clean:
        rm -f $(OBJS) $(TARGET)

~
~
~
~
~
~
~
