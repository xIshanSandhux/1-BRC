#include <iostream>
#include <cstring>
using namespace std;


int main(){
    
    struct a{
        int min;
        unsigned int max;
        int16_t sum,count;
        // int8_t count;
    };
    
    struct b{
        int16_t min,max;
        // int16_t max;
        int sum;
        unsigned int count;
    };

    cout<<sizeof(int16_t)<<endl;
    cout<<sizeof(int)<<endl;

    cout<<2%1<<endl;

    
    struct c{
        int min;
        int16_t max;
        unsigned int sum;
        int16_t count;
    };
    cout<<sizeof(a)<<endl;
    cout<<sizeof(b)<<endl;
    cout<<sizeof(c)<<endl;
    return 0;
}

