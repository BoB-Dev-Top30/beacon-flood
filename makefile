# Compiler 설정
CXX = g++

# 링크할 라이브러리
LDLIBS += -lpcap -ltins

# 기본 타겟
all: beacon-flood

# beacon-flood 타겟 빌드 규칙
beacon-flood: beacon-flood.o
	$(CXX) -o beacon-flood beacon-flood.o $(LDLIBS)

# .cpp 파일을 .o 파일로 컴파일하는 일반 규칙
%.o: %.cpp
	$(CXX) -c $<

# clean 규칙
clean:
	rm -f beacon-flood *.o


