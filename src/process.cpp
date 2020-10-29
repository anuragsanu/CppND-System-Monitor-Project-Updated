#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "format.h"
#include "linux_parser.h"
#include <unistd.h>

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return std::stoi(pd);}

// algorithm used is from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    const std::string stat_dir(LinuxParser::kProcDirectory + pd  + LinuxParser::kStatFilename);
    const std::string uptimefile = LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename;
    std::ifstream fs1(uptimefile);
    std::string timestr;
    std::getline(fs1 , timestr);
    std:: string localTimeinSeconds;
    std::istringstream ss1(timestr); 
    ss1 >> localTimeinSeconds;
    std::ifstream fs(stat_dir);
    string line;
    string value;
    int u_time;
    int s_time;
    int cu_time;
    int cs_time;
    int p_startTime;
    std::getline(fs , line);
    std::istringstream ss(line);
    int i =0;
     try
    {
    while(i != 22 && (ss >> value) && ++i)
    { 
     if(i == 13)
      {  
         u_time = std::stoi(value);
      }
      else if(i == 14)
      {
          s_time = std::stoi(value);
      }
      else if(i == 15)
      {
          cu_time = std::stoi(value);
      }
      else if(i == 16)
      {
          cs_time = std::stoi(value);
      }
      else if(i == 21)
      {
          p_startTime = std::stoi(value);
      }
    }
    }catch(std::exception e)
    {
    }
 
    int totaltime = u_time + s_time + cu_time + cs_time;
    totaltime = totaltime/sysconf(_SC_CLK_TCK);
    int process_total_time = std::stoi(localTimeinSeconds) - (p_startTime/sysconf(_SC_CLK_TCK));

    return totaltime/double(process_total_time);

}

// TODO: Return the command that generated this process
string Process::Command() { 
    const std::string command_dir(LinuxParser::kProcDirectory +  pd + LinuxParser::kCmdlineFilename);
    std::ifstream fs(command_dir);
    std::string command;
    std::getline(fs , command);
    return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    const std::string  memory_dir(LinuxParser::kProcDirectory + pd + LinuxParser::kStatusFilename);
    std::ifstream fs(memory_dir);
    std::string line;
    std::string key;
    std::string value;
    while(std::getline(fs, line))
    {
        std::istringstream ss(line);
        ss >> key >> value;
        if(key == "VmSize:")
        {
            int memorySize  = std::stoi(value);
            memorySize = memorySize/1000;
            return to_string(memorySize);
        }

    }
    return string();
 }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    const std::string statusFile = LinuxParser::kProcDirectory + pd + LinuxParser::kStatusFilename;
    std::ifstream pfs(LinuxParser::kPasswordPath);
    std::ifstream fs(statusFile);
    std::string line;
    std::string key1 , value1;
    std::string key2 , value2 , value3;
    while(std::getline(fs , line))
    {
        std::istringstream ss(line);
        ss >> key1 >> value1;
        if(key1 == "Uid:")
        {
            std::string pline;
            while(std::getline(pfs , pline))
            {
                std::replace(pline.begin() , pline.end() , ':', ' ');
                std::istringstream ss1(pline);
                ss1 >> key2 >> value2 >> value3;
                if(value3 == value1)
                {
                    return key2;
                }

            }
            
        }
    }


    return string(); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    const std::string uptime_dir(LinuxParser::kProcDirectory + pd  + LinuxParser::kStatFilename);
    std::ifstream fs(uptime_dir);
    long uptime;
    std::string time;
    std::getline(fs , time);
    std::istringstream ss(time);
    int i =0;
    while(i != 22 && (ss >> time) && ++i);
    try
    {
      uptime = std::stol(time)/sysconf(_SC_CLK_TCK);
    }catch(std::exception e)
    {
    }
 
    return uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a1 ) const {
       return (std::stoi(a1.pd) < std::stoi(this->pd));
}
