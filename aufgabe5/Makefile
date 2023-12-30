CXX = /usr/bin/g++
LDFLAGS =
CPPFLAGS = 
INC =
CXXFLAGS = -std=c++17 -Wall -pedantic -O2 -D_GLIBCXX_ASSERTIONS -g -fsanitize=address -fopenmp
# -D_GLIBCXX_DEBUG // bad for openmp performance


%.o: %.cpp QGramIndex.h
	${CXX} ${CXXFLAGS} -I . -c $*.cpp
	

aufgabe5_main: QGramIndex.o aufgabe5_main.o a5_util.o
	${CXX} ${CXXFLAGS} -I . $^ -o aufgabe5_main

aufgabe5_test: QGramIndex.o  aufgabe5_test.o a5_util.o
	${CXX} ${CXXFLAGS} -I . $^ -o  aufgabe5_test
  
