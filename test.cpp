#include <iostream>
#include <thread>
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

void parseLine(char* start, char* mid){
    string_view station(start,mid-start-1);

    bool neg = (*mid=='-');
    if (neg) mid++;

    int16_t t=0;
    while(*mid!='\n'){
        if(*mid!='.'){
            t = t*10+ (*mid-'0');
        }
        mid++;
    }
    if (neg) t*=-1;
    db.enter(station,t);
}

 
int main(){
    // The system I am working on has 4 threads that I can use, therefore
    // I am hardcoding them
    int totalThreads = 4;    
    cout<<"Total threads: "<<totalThreads<<endl;
    
    int fd = open("measurements10m.txt",O_RDONLY);
    struct stat st;
    fstat(fd,&st);
    size_t fileSize = st.st_size;
    cout<<"total file size: "<<fileSize<<endl;
    //based chunk size for each thread
    size_t totalChunkSize = fileSize/totalThreads;
    cout<<"Base chunk size per thread: "<<totalChunkSize<<endl;
    
    char* fileFull = (char*) mmap(NULL,fileSize,PROT_READ,MAP_PRIVATE,fd,0);
    
    size_t temp = totalChunkSize;
    char* chunks[totalThreads];
    int chunkSize[totalThreads];
    for(int i=0;i<totalThreads;i++){
        cout<<"thread: "<<i<<endl;
        if (i==0){
            while(*(fileFull+temp)!='\n'){
                temp++;
            }
            chunks[i] = (fileFull+temp);
        }else{
            while(*(chunks[i-1]+temp)!='\n'){
                if(*(chunks[i-1]+temp)=='\0'){
                    int counter=1;
                    while(*(chunks[i-1]+ counter)!='\0'){
                      counter++;
                    }
                    chunks[i] = chunks[i-1]+counter-1;
                    break;
                      }
                temp++;
            }
            chunks[i] = (chunks[i-1]+temp);
        }
        cout<<*(chunks[i])<<endl;
        temp = totalChunkSize-(temp-totalChunkSize)+1; 
    }

    char* lineStart= fileFull;
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
            parseLine(lineStart,midLine+1);
            counter+=(lineEnd-lineStart);
            lineStart=lineEnd+1;
            if(*lineStart=='\n' || *lineStart=='\0') break;
        }
    }

    munmap(fileFull,fileSize);
    close(fd);

    return 0;
}

