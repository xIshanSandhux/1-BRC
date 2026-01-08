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
    for(int i=0;i<totalThreads-1;i++){
        cout<<"hello"<<endl;
        cout<<"value of temp ebfore moving until the new line from thread: "<<i<<"is: "<<temp<<endl;
        if (i==0){
            while(*(fileFull+temp)!='\n'){
                temp++;
            }
            chunks[i] = (fileFull+temp);
            //cout<<"hey after storing chunk 0"<<endl;
            //cout<<*(chunks[i])<<endl;
            //cout<<temp<<endl;
        }else{
            while(*(chunks[i-1]+temp)!='\n'){
              //  cout<<*(chunks[i-1]+temp)<<"for thread number: "<<i<<endl;
                temp++;
            }
            //cout<<"value of temp from thread: "<<i<<"is: "<<temp<<endl;
            chunks[i] = (chunks[i-1]+temp);
            if (i==3){
                cout<<"i==3"<<endl;
            //cout<<chunks[i];
            }
            //cout<<"hey after storing chunk other than 0"<<endl;
            //cout<<*(chunks[i]+1)<<endl;
          //  cout<<temp<<endl;
        }
        //cout<<"total chunks left: "<<totalChunkSize-temp<<endl;
        temp = totalChunkSize-(temp-totalChunkSize)+1; 
        cout<<"new value of temp: "<<temp<<endl;
    }
    //cout<<*(file+10)<<endl;

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

