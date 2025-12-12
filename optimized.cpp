#include <iostream>
#include <fcntl.h>
#include <cstdio>
#include <chrono>
using namespace std;

int main(){

    auto start = std::chrono::high_resolution_clock::now();
    // size_t bytesRead, bufferSize = 4096;
    // char *buffer = new char[bufferSize];

    
    int fd = open("sample.txt",O_RDONLY);
    cout<<fd <<endl;
    // delete buffer;

    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() << " ns\n";
    return 0;
}