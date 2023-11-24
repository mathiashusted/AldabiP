CXX = /usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -g -Wall -pedantic -O2 -D_GLIBCXX_DEBUG -fsanitize=address

%.o: %.cpp
	${CXX} ${CXXFLAGS} -I . -c $*.cpp

aufgabe3_main: Alignment.o aufgabe3_main.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe3_main

aufgabe3_test: Alignment.o aufgabe3_test.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe3_test

