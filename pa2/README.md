
Compile program using:
g++ -std=c++17 -O2 -Wall -Wextra strassen.cpp -o strassen -lm -lpthread

g++ -std=c++17 -o matrix matrix.cc

g++ -std=c++17 -o randmst randmst.cc

g++ -Xpreprocessor -fopenmp matrix.cc  -o  hellosmp
g++ -Xpreprocessor -fopenmp matrix.cc -lomp -o test
g++ matrix.cc -fopenmp

g++ -Xclang -fopenmp matrix.cc -lomp -o test

g++ -c matrix.cc -o matrix.o -fopenmp