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
    totaltime = totaltime - vec[static_cast<int>(LinuxParser::CPUStates::kGuest_)] - vec[static_cast<int>(LinuxParser::CPUStates::kGuestNice_)];
    int idle = vec[static_cast<int>(LinuxParser::CPUStates::kIdle_)] + vec[static_cast<int>(LinuxParser::CPUStates::kIOwait_)];
    float cpuUtilization = ((totaltime -prevTotal) - (idle - prevtotalIdle))/float(totaltime - prevTotal);
    prevuser = vec[static_cast<int>(LinuxParser::CPUStates::kUser_)];
    prevnice = vec[static_cast<int>(LinuxParser::CPUStates::kNice_)];
    prevsystem = vec[static_cast<int>(LinuxParser::CPUStates::kSystem_)];
    previdle = vec[static_cast<int>(LinuxParser::CPUStates::kIdle_)];
    previowait = vec[static_cast<int>(LinuxParser::CPUStates::kIOwait_)];
    previrq = vec[static_cast<int>(LinuxParser::CPUStates::kIRQ_)];
    prevsoftirq = vec[static_cast<int>(LinuxParser::CPUStates::kSoftIRQ_)];
    prevsteal = vec[static_cast<int>(LinuxParser::CPUStates::kSteal_)];
    prevTotal = totaltime;
    prevtotalIdle = previdle + previowait;
    return  cpuUtilization;
    }
    return 0.0;
}
