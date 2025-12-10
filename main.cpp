#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream sampleFile("sample.txt");
    std::string line;
    std::vector<string> values;
    std::map<string,std::vector<string>> hMap;

    // checking if the file exists or not
    if(!sampleFile.is_open()){
        cout<<"File Not Found\n";
        // indicating that its an error
        return 1;

    }

    size_t pos;
    while(std::getline(sampleFile,line)){
        pos = line.find(";");
        if (hMap.find(line.substr(0,pos))!=hMap.end()){
            hMap[line.substr(0,pos)].push_back(line.substr(pos+1));
        }else{
            hMap[line.substr(0,pos)] = values;
            hMap[line.substr(0,pos)].push_back(line.substr(pos+1));
        }
        // cout << line.substr(pos+1) << endl; 
    }

    for (auto it = hMap.begin(); it != hMap.end(); ++it) {
        std::cout << it->first << endl;
    }


    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() << " ns\n";


    return 0;
}