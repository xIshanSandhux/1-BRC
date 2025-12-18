#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <chrono>
#include <cstring>
#include <unordered_map>
using namespace std;


int main(){

    
    int fd = open("sample2.txt",O_RDONLY);
    cout<<"main function called\n";
    struct stat st;
    fstat(fd,&st);
    size_t fileSize = st.st_size;
    cout<<"File Size: "<<fileSize<<endl;
    char* strtry = "Berlin;12.0";

    char* file = (char*) mmap(NULL,fileSize,PROT_READ,MAP_PRIVATE,fd,0);
    cout<<"Reading first char: "<<*file<<endl;

    size_t curr_pos=0;
    char* lineStart= file;
    cout<<"Line start: "<<*lineStart<<endl;
    char* lineEnd;
    char* midLine;

    size_t lineSize=0,counter=0;
    while(counter<=fileSize){
        cout<<"hello\n";
        lineEnd = strchr(lineStart,'\n');
        if(lineEnd!=nullptr){
            cout<<"LineEnd: "<<*(lineEnd)<<endl;
        }
        // while(*lineStart!=*lineEnd){
        //     cout<<*lineStart;
        //     lineStart++;
        // }
        // cout<<"line End pointer: "<<(*lineEnd-1);
        if(lineEnd==nullptr) {
            cout<<"nothing found here returned nullpointer"<<endl;
            break;
        }
        
        counter+=lineSize;
        lineStart=lineEnd+1;
    }
    munmap(file,fileSize);
    close(fd);

    return 0;
}

