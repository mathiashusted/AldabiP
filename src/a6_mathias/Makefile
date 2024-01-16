CXX = /usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -Wall -pedantic -O2 -D_GLIBCXX_ASSERTIONS -g -fsanitize=address
# -D_GLIBCXX_DEBUG // bad for openmp performance


%.o: %.cpp ACTrie.hpp
	${CXX} ${CXXFLAGS} -I . -c $*.cpp


aufgabe6_main: ACTrie.o aufgabe6_main.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe6_main

aufgabe6_test: ACTrie.o  aufgabe6_test.o
	${CXX} ${CXXFLAGS} -I . $^ -o  aufgabe6_test

