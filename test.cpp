#include <iostream>
#include <cstring>
#include <functional>
using namespace std;


int main(){
    std::hash<string> hello;
    int16_t c = 10000;
    // size_t t=hash<string>{}("hello"); 
    // cout<< (t & (c-1))<<endl;
    size_t i = (hash<string>{}("hello world"))&16383;
    cout<<i<<endl;
    // cout<<t<<endl;
    return 0;
}

