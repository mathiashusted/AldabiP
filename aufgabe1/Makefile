CXX = /usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -g -Wall -pedantic -O2 -D_GLIBCXX_DEBUG -fsanitize=address

%.o: %.cpp
	${CXX} ${CXXFLAGS} -I . -c $*.cpp

aufgabe1_main: aufgabe1.o aufgabe1_main.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe1_main

aufgabe1_test: aufgabe1.o aufgabe1_test.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe1_test

