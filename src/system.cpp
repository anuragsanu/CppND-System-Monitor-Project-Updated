#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include <iostream>
#include <dirent.h>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

static const std::string pretty_name{"PRETTY_NAME"};


System::System()
{ 
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();
    DIR* dir = opendir(LinuxParser::kProcDirectory.c_str());
    struct dirent* file;
    while((file = readdir(dir)) != nullptr)
    {
        
        if(file->d_type == DT_DIR)
        {
            const std::string dirName(file->d_name);
            if(std::all_of(dirName.begin() , dirName.end() , isdigit))
            {
                Process p;
                p.pd = dirName;
                processes_.emplace_back(p);
            }
        }
    }
    closedir(dir);
    RearrangeProcess();
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    const std::string fileName = LinuxParser::kProcDirectory + LinuxParser::kVersionFilename;
    std::string os , version , kernel;
    std::ifstream fs(fileName);
    std::string line ;
    std::getline(fs, line);
    std::istringstream ss(line);
    ss >> os >> version >> kernel;
    return kernel;
}

template<class T1 , class T2> 
float caluclateMemory(const std::vector<std::pair<T1 , T2>>& vec)
{
    int totalMemory = vec[0].second;
    int freeMemory = vec[1].second;
    float memusedPercent = ((totalMemory - freeMemory)/float(totalMemory));
    return memusedPercent;
}

// TODO: Return the system's memory utilization
// Algorithm used is from https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
float System::MemoryUtilization() {
    const std::string fileName = LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename;
    std::ifstream fs(fileName);
    std::string line;
    std::string key ;
    std::string value;
    std::vector<std::pair<string , int>> vec(2);
    for(int i = 0 ; i < 2 ; ++i)
    {
        std::getline(fs , line);
        std::istringstream ss(line);
        ss >> key >> value;
        vec[i].first = key;
        vec[i].second = std::stoi(value);
        
    }
    return caluclateMemory<string , int>(vec);
    
}


void System::RearrangeProcess()
{
     std::sort(processes_.begin() , processes_.end() , [](const Process& p1 , const Process& p2){
         return (p1 < p2);
         });
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
      std::ifstream fs(LinuxParser::kOSPath);
      std::string line;
      if(fs.is_open())
      {
          while(std::getline(fs , line))
          {
             std::replace(line.begin() , line.end() ,' ', '_');
             std::replace(line.begin() , line.end() , '=', ' ');
             std::replace(line.begin() , line.end() , '"', ' ');
             std::istringstream inputstring(line);
             std::string key , value;
             inputstring >> key >> value;
             if(key == pretty_name)
             {
                 std::replace(value.begin() , value.end() , '_', ' ');
                 return value;
             }

          }
          
      }
      return "no operating system name";
 }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    const std::string statFile = LinuxParser::kProcDirectory + LinuxParser::kStatFilename ;
    std::ifstream fs(statFile);
    std::string line;
    std::string key;
    std::string value;
    
    while(std::getline(fs , line))
    {
        std::istringstream ss(line);
        ss >> key >> value;
        if(key == "procs_running")
        {
            return std::stoi(value);
        }
    }
    
    return 0;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    const std::string statFile = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
    std::ifstream fs(statFile);
    string line;
    string key , value ;
    while(std::getline(fs , line))
    {
       std::istringstream ss(line);
       ss >> key >> value;
       if(key == "processes")
       {
           return std::stoi(value);
       }

    }  
    return 0; 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    
    const std::string uptimefile = LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename;
    std::ifstream fs(uptimefile);
    std::string timestr;
    std::getline(fs , timestr);
    std:: string localTimeinSeconds;
    std::istringstream ss(timestr); 
    ss >> localTimeinSeconds;
    long int localTime = std::stol(localTimeinSeconds);
    return localTime;
    
}
