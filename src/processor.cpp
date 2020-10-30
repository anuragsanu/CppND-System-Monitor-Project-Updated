#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <unistd.h>


using std::string ;
using std::vector;

// TODO: Return the aggregate CPU utilization
// the algorithm used is from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
    const std::string statFile =  LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
    std::ifstream fs(statFile);
    if(fs.is_open()){
    std::string line;
    std::getline(fs , line);
    std::istringstream ss(line);
    std::string key;
    ss >> key;
    std::string value;
    vector<int> vec;
    while(ss >> value)
    {
       vec.emplace_back(std::stoi(value));
    }
    int totaltime = std::accumulate(vec.begin() ,vec.end() , 0);
    totaltime = totaltime - vec[8] - vec[9];
    int idle = vec[3] + vec[4];
    float cpuUtilization = ((totaltime -prevTotal) - (idle - prevtotalIdle))/float(totaltime - prevTotal);
    prevuser = vec[0];
    prevnice = vec[1];
    prevsystem = vec[2];
    previdle = vec[3];
    previowait = vec[4];
    previrq = vec[5];
    prevsoftirq = vec[6];
    prevsteal = vec[7];
    prevTotal = totaltime;
    prevtotalIdle = previdle + previowait;
    return  cpuUtilization;
    }
    return 0.0;
}
