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
#include <functional>
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

};

struct DB{
    DB(): stations_{}, stats_{} {}

    void enter(string_view station, int16_t temp){
        size_t index = slotFind(station);

        if(stations_[index].empty()){
            stations_[index]=station;
            stats_[index].intialize(temp);
        }else{
            stats_[index].update(temp);
        }

    }

    size_t slotFind(string_view station){
        size_t index = (hash<string_view>{}(station))&(32767);

        while((stations_[index].empty())==false){
            if(stations_[index]==station){
                return index;
            }
            index = (index+1)%32767;
           
        }
        return index;
    }

    array<string_view,32768> stations_;
    array<Stats,32768> stats_;
};

DB db;

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
    db.enter(station,t);
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

