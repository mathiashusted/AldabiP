CXX=/usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -Wall -pedantic -O3 -D_GLIBCXX_ASSERTIONS -g -fsanitize=address

%.o: %.cpp
	${CXX} ${CXXFLAGS} -I . -c $*.cpp
	

aufgabe2_main: aufgabe2.o aufgabe2_main.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe2_main

aufgabe2_test: aufgabe2.o aufgabe2_test.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe2_test
  
aufgabe2_bench: aufgabe2.o aufgabe2_bench.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe2_bench
