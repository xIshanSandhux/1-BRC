#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <chrono>
using namespace std;

int main(){

    auto start = std::chrono::high_resolution_clock::now();
    
    int fd = open("sample.txt",O_RDONLY);
    struct stat st;
    fstat(fd,&st);
    size_t fileSize = st.st_size;

    char* file = (char*) mmap(NULL,fileSize,PROT_READ,MAP_PRIVATE,fd,0);

    size_t curr_pos=0, lineStart,lineEnd;
    while(curr_pos<fileSize){
        lineStart=curr_pos;
        while(curr_pos<fileSize && file[curr_pos]!= '\n'){
            curr_pos++;
        }
        if(curr_pos==fileSize){
            break;
        }
        lineEnd=curr_pos;
        while(lineStart<lineEnd){
            lineStart++;
        }
        curr_pos++;
    }
    munmap(file,fileSize);
    close(fd);

    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() << " ns\n";
    return 0;
}