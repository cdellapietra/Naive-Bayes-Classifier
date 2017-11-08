NaiveBayes : main.cpp
	g++ -std=c++11 -Wall -pedantic -c main.cpp
	g++ -std=c++11 -Wall -pedantic -o NaiveBayes main.cpp

clean :
	rm -f *.o NaiveBayes core.* 
