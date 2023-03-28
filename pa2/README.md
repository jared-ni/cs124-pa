
Compile program using:
g++ -std=c++17 -o matrix matrix.cc -fopenmp
/opt/homebrew/opt/llvm/bin/clang -fopenmp -L/opt/homebrew/opt/llvm/lib matrix.cc
clang -Xpreprocessor -fopenmp matrix.cc -lomp