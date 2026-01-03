#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

int main(){
    std::ifstream sampleFile("measurements10m.txt");
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
        
        double avg () const{
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

    

    // for(const auto& [key,value]:hMap){
    //     // cout.precision(2);
    //     cout << key << ";min="<<value.min<<";max="<<value.max<<";avg="<< value.avg() <<endl;
    // }

    auto end = std::chrono::high_resolution_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // std::cout << ns.count() << " ns\n";
    // cout<<ms.count()<<" ms\n";

    return 0;
}
