#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <string_view>
#include <charconv>
#include <typeinfo>
using namespace std;

struct Stats{
    int sum=0;
    unsigned int count=0;
    int16_t min,max;
    
    void intialize(int16_t temp){
        max = temp;
        min = temp;
        sum+=temp;
        count++;
    }

    void update(int16_t temp){
        if(max<temp) max=temp;
        if(min>temp) min=temp;
        sum+=temp;
        count++;
    }

    void printVals(){
        cout<<max<<" "<<min<<" " <<" "<<sum<<" "<<count<<endl;
    }

};
// need to create a custom flat hashmap cause this is slow
unordered_map<string_view,Stats> db;

void parseLine(char* start, char* mid, char* end){
    string_view station(start,mid-start-1);
    size_t s = end-mid;

    int16_t t=0;
    for(size_t i=0;i<s;i++){
        if(*(mid+i)!='.' && i==0 && *(mid+i)!='-'){
            t =  *(mid+i)-'0';
        }else if (*(mid+i)!='.' && *(mid+i)!='-'){
            t = t*10+ (*(mid+i)-'0');
        }
    }
    if(*mid=='-') t*=-1;

    if(db.find(station)==db.end()){
        db[station].intialize(t);
    }else{
        db[station].update(t);
    }
}

 
int main(){
    
    int fd = open("measurements10m.txt",O_RDONLY);
    struct stat st;
    fstat(fd,&st);
    size_t fileSize = st.st_size;
    char* file = (char*) mmap(NULL,fileSize,PROT_READ,MAP_PRIVATE,fd,0);

    char* lineStart= file;
    char* lineEnd;
    char* midLine;

    size_t counter=0;
    db.reserve(8858);
    while(counter<fileSize){
        lineEnd = (char*)memchr(lineStart,'\n',fileSize);
        if(lineEnd==nullptr) {
            break;
        }else{
            midLine = (char*)memchr(lineStart,';',fileSize);
            if(midLine==nullptr) break;
            parseLine(lineStart,midLine+1,lineEnd);
            counter+=(lineEnd-lineStart);
            lineStart=lineEnd+1;
            if(*lineStart=='\n' || *lineStart=='\0') break;
        }
    }
    munmap(file,fileSize);
    close(fd);

    return 0;
}

