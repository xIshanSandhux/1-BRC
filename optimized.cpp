#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <chrono>
#include <cstring>
#include <unordered_map>
#include <string_view>
#include <charconv>
#include <typeinfo>
using namespace std;


struct Stats{
    float sum=0;
    float max;
    float min;
    int count=0;

    Stats(float temp){
        max = temp;
        min = temp;
        sum+=temp;
        count++;
    }

    void update(float temp){
        if(max<temp) max=temp;
        if(min>temp) min=temp;
        sum+=temp;
        count++;
    }

};

unordered_map<string,Stats> db;

// int16_t parseTemp(string_view temp){

//     for(auto c : temp){
//         cout<<c<<endl;
//     }
//     return 0;
// }



void parseLine(char* start, char* mid, char* end){
    cout<<"called"<<endl;
    string_view station(start,mid-start-1);
    cout<<station.size()<<endl;
    string_view temp(mid,end-mid);

    int16_t t;
    from_chars(temp.data(),temp.data()+temp.size(),t);
    // cout<<t<<endl;

    cout<<"printing t: "<<t<<endl;
    // cout<< typeid(t)<<endl;
    // cout<< typeid(t)<<endl;
    cout<<temp.size()<<endl;
    cout<<station<<endl;
    
    cout<<temp<<endl;
    cout<<"call end"<<endl;
}




int main(){

    auto start = std::chrono::high_resolution_clock::now();
    
    int fd = open("sample2.txt",O_RDONLY);
    struct stat st;
    fstat(fd,&st);
    size_t fileSize = st.st_size;
    char* file = (char*) mmap(NULL,fileSize,PROT_READ,MAP_PRIVATE,fd,0);

    size_t curr_pos=0;
    char* lineStart= file;
    char* lineEnd;
    char* midLine;
    char* fraction;

    size_t lineSize=0,counter=0;
    while(counter<=fileSize){
        lineEnd = (char*)memchr(lineStart,'\n',fileSize);
        cout<< '8'-'0'<<": printing int vals rty"<<endl;

        if(lineEnd==nullptr) {
            cout<<"nothing found here returned nullpointer"<<endl;
            break;
        }else{
            midLine = (char*)memchr(lineStart,';',fileSize);
            fraction = (char*)memchr(midLine,'.',fileSize);
            parseLine(lineStart,midLine+1,lineEnd);
        }
        lineSize=lineEnd-lineStart;
        counter+=lineSize;
        lineStart=lineEnd+1;
    }
    munmap(file,fileSize);
    close(fd);

    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() << " ns\n";
    return 0;
}

