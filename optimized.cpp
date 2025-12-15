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

    size_t curr_pos=0, lineStart,lineEnd;
    size_t* line;
    while(curr_pos<fileSize){
        lineStart=curr_pos;
        line = (size_t*)memchr(file,'\n',fileSize-curr_pos);
        char* line1 = (char*)memchr(file,'\n',fileSize-curr_pos);
        
        // lineEnd= *line;
        if(line==nullptr) break;

        while(lineStart<*line){
            cout<<file[lineStart];
            lineStart++;
        }


        // cout<<line<<endl;
        curr_pos=*line+1;
        // curr_pos = line;
        // while(curr_pos<fileSize && file[curr_pos]!= '\n'){
        //     curr_pos++;
        // }
        // if(curr_pos==fileSize){
        //     break;
        // }
        // lineEnd=curr_pos;
        // while(lineStart<lineEnd){
        //     lineStart++;
        // }
        // curr_pos++;
    }
    munmap(file,fileSize);
    close(fd);

    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() << " ns\n";
    return 0;
}