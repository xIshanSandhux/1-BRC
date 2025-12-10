#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

int main(){
    std::ifstream sampleFile("sample.txt");
    std::string line;
    size_t pos;
    std::string station;
    float temp;


    struct tempStuff{
        float min;
        float max;
        float sum;
        int count;

        void update(float temp){
            if(min>temp) min = temp;
            if(max<temp) max = temp;
            sum+=temp;
            count++;
        }
        void initalize(float temp){
            min = temp;
            max = temp;
            sum = temp;
            count = 1;
        }
        
        float avg(){
            return sum/count;
        }
    };



    std::map<string,tempStuff> hMap;

    auto start = std::chrono::high_resolution_clock::now();
    // checking if the file exists or not
    if(!sampleFile.is_open()){
        cout<<"File Not Found\n";
        // indicating that its an error
        return 1;

    }

        while(std::getline(sampleFile,line)){
        pos = line.find(";");
        station = line.substr(0,pos);
        temp = std::stof(line.substr(pos+1));
        if (hMap.find(station)!=hMap.end()){
            hMap[station].update(temp);
        }else{
            hMap[station].initalize(temp);
        }
    }

    
    auto end = std::chrono::high_resolution_clock::now();
    for (auto it = hMap.begin(); it != hMap.end(); ++it) {
        std::cout << it->first << endl;
    }


    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << ns.count() << " ns\n";


    return 0;
}