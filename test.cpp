#include <iostream>
#include <thread>
using namespace std;

void t(int r){
    for(int i=0;i<r;i++){
        cout<<r<<endl;
    }
}
int x = 10;
void worker(){
    x++;
    cout<<x<<endl;
}

void worker1(){
    int local = 10;
    local++;
    cout<<local<<endl;
}

void w(string a){
    cout<<a<<endl;
}

int main(){
    int thread = thread::hardware_concurrency();
    std::thread t1 (worker);
    cout<<t1.get_id()<<" : thread 1 id"<<endl;
    std::thread t2 (worker);
    cout<<t2.get_id()<<" : thread 2 id"<<endl;

    std::thread t3(worker1);
    std::thread t4(worker1);
    
    if(t1.joinable()) t1.join();
    if(t2.joinable()) t2.join();
    if(t3.joinable()) t3.join();
    if(t4.joinable()) t4.join();
    cout<<thread<<endl;
    return 0;
}

