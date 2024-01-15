CXX = /usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -g -Wall -pedantic -O3 -fopenmp -msse4

%.o: %.cpp AlignmentChallenge.h
	${CXX} ${CXXFLAGS} -I . -c $*.cpp

bench_AC: AlignmentChallenge.o bench_AC.o
	${CXX} ${CXXFLAGS} -I . $^ -o bench_AC


