#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <chrono>
#include <cstring>
using namespace std;

int main(){

    auto start = std::chrono::high_resolution_clock::now();
    
    int fd = open("sample.txt",O_RDONLY);
    struct stat st;
    fstat(fd,&st);
    size_t fileSize = st.st_size;

    char* file = (char*) mmap(NULL,fileSize,PROT_READ,MAP_PRIVATE,fd,0);

    size_t curr_pos=0;
    char* lineStart= file;
    char*lineEnd;

    size_t lineSize=0,counter=0;
    while(lineEnd!=nullptr){
        lineEnd = (char*)memchr(lineStart,'\n',fileSize-lineSize);
        if(lineEnd==nullptr) break;
        lineSize=lineEnd-lineStart;
        while(counter<lineSize){
            lineStart++;
            counter++;
        }
        counter=0;
        lineStart=lineEnd+1;
    }
    munmap(file,fileSize);
    close(fd);

    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() << " ns\n";
    return 0;
}