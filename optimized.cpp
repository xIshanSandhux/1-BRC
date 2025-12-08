#include <iostream>
#include <cstdio>
#include <chrono>
using namespace std;

int main(){

    auto start = std::chrono::high_resolution_clock::now();
    size_t bytesRead, bufferSize = 4096;
    char *buffer = new char[bufferSize];

    FILE* fp = fopen("sample.txt","rb");
    if(!fp){
        cerr<<"file not found";
        return 1;
    }

    while((bytesRead=fread(buffer,sizeof buffer[0],bufferSize,fp))>0){
        if(bytesRead==0){
            break;
        }
    }
    fclose(fp);
    delete buffer;

    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() << " ns\n";
    return 0;
}