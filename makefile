
CXX = g++

LDLIBS = 


all: beacon-flood

# beacon-flood 타겟 빌드 규칙
beacon-flood: beacon_frame.o main.o utils.o
	$(CXX) -o beacon-flood beacon_frame.o main.o utils.o $(LDLIBS)

%.o: %.cpp
	$(CXX) -c $<

clean:
	rm -f beacon-flood *.o

