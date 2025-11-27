CXX = g++
CXXFLAGS = -std=c++17 -fPIC -Wall -O2 -Iinclude
LDFLAGS = -pthread

SRC = src/parallel_scheduler.cpp
HDR = include/parallel_scheduler.h include/parallel_scheduler.tpp
LIB = libparallel_scheduler.so
DEMO = demo

all: $(LIB) $(DEMO)

# Сборка shared library
$(LIB): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -shared -o $(LIB) $(SRC) $(LDFLAGS)

# Сборка demo-приложения
$(DEMO): demo.cpp $(LIB) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(DEMO) demo.cpp -L. -lparallel_scheduler $(LDFLAGS)

clean:
	rm -f $(LIB) $(DEMO)

