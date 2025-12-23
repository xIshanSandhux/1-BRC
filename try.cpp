#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
using namespace std;

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream sampleFile("sample.txt");
    std::string line;
    size_t pos;
    float temp;
    // std::string line;

    // checking if the file exists or not
    if(!sampleFile.is_open()){
        cout<<"File Not Found\n";
        // indicating that its an error
        return 1;

    }
    

    while(std::getline(sampleFile,line)){
        // cout<<line<<endl;
        pos = line.find(";");
        temp = std::stof(line.substr(pos+1));
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << ns.count() << " ns\n";
    cout<<ms.count()<<" ms\n";


    return 0;
}