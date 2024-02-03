CXX = /usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -Wall -pedantic -O2 -D_GLIBCXX_ASSERTIONS -g -fsanitize=address
# -D_GLIBCXX_DEBUG // bad for openmp performance

%.o: %.cpp PDA.hpp
	${CXX} ${CXXFLAGS} -I . -c $*.cpp
	

aufgabe7_main: PDA.o aufgabe7_main.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe7_main

aufgabe7_test: PDA.o  aufgabe7_test.o
	${CXX} ${CXXFLAGS} -I . $^ -o  aufgabe7_test
  
