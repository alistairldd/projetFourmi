CXX=g++
CXXFLAGS= -Wall -std=c++11 -O3
EXEC_FILES= main

all: main

clean:
	rm -f *.o $(EXEC_FILES) *.ppm movie.gif

test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c $<

coord.o: coord.cpp coord.hpp
	$(CXX) $(CXXFLAGS) -c $< 
    
fourmis.o: fourmis.cpp fourmis.hpp coord.hpp
	$(CXX) $(CXXFLAGS) -c $<


colonie.o : colonie.cpp coord.hpp fourmis.hpp colonie.hpp
	$(CXX) $(CXXFLAGS) -c $<

place.o : place.cpp place.hpp coord.hpp fourmis.hpp colonie.hpp
	$(CXX) $(CXXFLAGS) -c $<
    
affichage.o : affichage.cpp place.hpp coord.hpp fourmis.hpp affichage.hpp
	$(CXX) $(CXXFLAGS) -c $<
    
main.o : main.cpp place.hpp affichage.hpp coord.hpp fourmis.hpp colonie.hpp
	$(CXX) $(CXXFLAGS) -c $<
    
main : main.o affichage.o place.o fourmis.o coord.o test.o colonie.o
	$(CXX) -o $@ $^