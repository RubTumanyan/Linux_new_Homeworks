CXX = g++
CXXFLAGS = -std=c++17 -fPIC -O2
LDFLAGS = -pthread

all: demo-application

parallel_scheduler.so: parallel_scheduler.cpp parallel_scheduler.hpp
        $(CXX) $(CXXFLAGS) -shared -o $@ parallel_scheduler.cpp $(LDFLAGS)

demo-application: demo.cpp parallel_scheduler.so
        $(CXX) $(CXXFLAGS) -o $@ demo.cpp -L. -lparallel_scheduler $(LDFLAGS)

clean:
        rm -f demo-application parallel_scheduler.so
