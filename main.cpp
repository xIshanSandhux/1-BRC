#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream sampleFile("sample.txt");
    std::string line;

    // checking if the file exists or not
    if(!sampleFile.is_open()){
        cout<<"File Not Found\n";
        // indicating that its an error
        return 1;

    }

    while(std::getline(sampleFile,line)){
        cout << line<<"\n";
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << ms.count() << " ms\n";

    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << us.count() << " µs\n";

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() << " ns\n";


    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time: " << elapsed.count() << " seconds\n";

    return 0;
}