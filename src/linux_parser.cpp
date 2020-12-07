#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include "format.h"

#include <unistd.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template<class RetType>
RetType LinuxParser::SearchInfo(std::string filePath, std::string keyToFind)
{
  RetType total = 0;
  std::ifstream file(filePath);
  if(file.is_open())
  {
    string line, key;
    while(std::getline(file, line))
    {
      std::istringstream stm(line);
      stm >> key >> total;
      if(key == keyToFind)
      {
        break;
      }
    }
  }

  return total;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{  
  long totalMemory;
  long avlblMemory;

  int ctr=0;

  std::ifstream file(kProcDirectory + kMeminfoFilename);
  if(file.is_open())
  {
    string line,key;
    long temp;
    
    while(std::getline(file, line))
    {
      if(ctr==2)
        break;

      std::istringstream stm(line);
      stm >> key >> temp;
      if(key == "MemTotal:")
      {
        totalMemory = temp;
        ctr++;
      }
      else if(key == "MemAvailable:")
      {
        avlblMemory = temp;
        ctr++;
      }
    }
  }
    
  if(ctr==2)
  {
    float res = (float)((totalMemory - avlblMemory)/(float)totalMemory);
    //std::cout<<"\nres:"<<res;
    return res;
  }

  return 0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  long upTime {0};
  std::ifstream file(kProcDirectory + kUptimeFilename);

  if(file.is_open())
  {
    string line;
    std::getline(file, line);
    std::istringstream lineStream(line);
    lineStream >> upTime;
    //std::cout<<"\nSTDBG::UpTime"<<upTime; 
  }

  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  long total {0};
  std::ifstream file(kProcDirectory + kStatFilename);

  if(file.is_open())
  {
    string line;
    std::getline(file, line);
    std::istringstream lineStream(line);
    string head;
  
    lineStream >> head;
    lineStream >> total;
  }

  return total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// Update: Did not need it as I used a different logic to calculate the CPU% of each Process
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{ 
  long active {0};
  std::ifstream file(kProcDirectory + kStatFilename);

  if(file.is_open())
  {
    string line;
    std::getline(file, line);
    std::istringstream lineStream(line);
    string head;
    int ctr = 3;
    lineStream >> head;

    while(ctr>0)
    {
      long temp;
      lineStream >> temp;
      active += temp;
      ctr--;
    }
  }

  return active;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  long idle {0};
  std::ifstream file(kProcDirectory + kStatFilename);

  if(file.is_open())
  {
    string line;
    std::getline(file, line);
    std::istringstream lineStream(line);
    string head;
    int ctr = 5;
    lineStream >> head;

    while(ctr>0)
    {
      long temp;
      lineStream >> temp;
      if(ctr>2)
      {
        ctr--;
        continue;
      }

      idle += temp;
      ctr--;
    }
  }

  return idle;
}

// TODO: Read and return CPU utilization
// Update: Not called. CpuUtilization calculated for each proc separately in CpuUtil
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  return SearchInfo<int>(kProcDirectory + kStatFilename, "processes");
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  return SearchInfo<int>(kProcDirectory + kStatFilename, "procs_running");
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid)
{ 
  std::string file {kProcDirectory};
  file += std::to_string(pid);
  file += kCmdlineFilename;
  //std::cout<<"\nFile Comd:"<<file;
  std::ifstream fileOpen(file);
  std::string cmd;

  if(fileOpen.is_open())
  {
    std::string line;
    std::getline(fileOpen, line);
    //std::cout<<"\nComd Line:"<<line;
    std::istringstream str(line);
    str>>cmd;
  }

  return cmd;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) 
{ 
  std::string file {kProcDirectory};
  file += std::to_string(pid);
  file += kStatusFilename;

  std::ifstream fileOpen(file);
  long ramkb = 0;

  if(fileOpen.is_open())
  {
    std::string line;
    while(std::getline(fileOpen, line))
    {
      std::istringstream str(line);
      std::string title;

      str>>title>>ramkb;

      if(title == "VmSize:")
        break;
    }

    fileOpen.close();
  }

  return std::to_string(ramkb/1024);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// Update: Was not needed so commenting. UID was found in User function itself
// string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  //First find the Uid
  std::string file {kProcDirectory};
  file += std::to_string(pid);
  file += kStatusFilename;

  std::ifstream fileOpen(file);
  long uid;

  if(fileOpen.is_open())
  {
    std::string line;
    while(std::getline(fileOpen, line))
    {
      std::istringstream str(line);
      std::string title;

      str>>title>>uid;

      if(title == "Uid:")
        break;
    }

    fileOpen.close();
  }

  std::ifstream pwdFile("/etc/passwd");

  if(pwdFile.is_open())
  {
    std::string line;
    while(std::getline(pwdFile, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      string uname, pwd;
      long uid2;

      std::istringstream str(line);

      str>>uname>>pwd>>uid2;

      if(uid2 == uid)
        return uname;

    }
  }

  return "Invalid";

}

float LinuxParser::CpuUtil(int pid)
{
  float usage = 0.0;

  std::string file {kProcDirectory};
  file += std::to_string(pid);
  file += kStatFilename;
  
  std::ifstream fileOpen(file);
  long utime, stime, cutime, cstime, starttime;
  long totalTime = 0;

  if(fileOpen.is_open())
  {
    std::string line;
    std::getline(fileOpen, line);
    
    std::istringstream str(line);
    std::string title, title2;
    
    long det;
    str>>det>>title>>title2;

    int ctr = 19;
    int i = 1;
    while(i<=ctr)
    {
      str>>det;

      switch(i)
      {
        case 11:
          utime = det;
          break;
        case 12:
          stime = det;
          break;
        case 13:
          cutime = det;
          break;
        case 14:
          cstime = det;
          break;
        case 19:
          starttime = det;
          break;

      }

      i++;
    }
    //std::cout<<"\nPid:"<<pid<<" utime:"<<utime<<" stime:"<<stime<<" cutime:"<<cutime<<" cstime:"<<cstime;
    totalTime = utime + stime + cutime + cstime;
    long upTime = UpTime();
    float elapsedTimeSec = upTime - (starttime/sysconf(_SC_CLK_TCK));
    //std::cout<<"\nUpTime:"<<elapsedTimeSec<<" totalTime:"<<totalTime;
    usage = ((totalTime / sysconf(_SC_CLK_TCK)) / elapsedTimeSec);
    //std::cout<<"\nUsage:"<<usage;
  }

  return usage;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid)
{ 
  std::string file {kProcDirectory};
  file += std::to_string(pid);
  file += kStatFilename;

  std::ifstream fileOpen(file);
  long starttime;
  long int elapsedTimeSec;

  if(fileOpen.is_open())
  {
    std::string line;
    std::getline(fileOpen, line);
    
    std::istringstream str(line);
    std::string title, title2;
    
    long det;
    str>>det>>title>>title2;

    int ctr = 19;
    int i = 1;
    while(i<=ctr)
    {
      str>>det;

      switch(ctr)
      {
        case 19:
          starttime = det;
          break;
      }

      i++;
    }

    long upTime = UpTime();
    //std::cout<<"\nPid:"<<pid<<" upTime:"<<upTime<<" startTime:"<<starttime;
    elapsedTimeSec = upTime - (starttime/sysconf(_SC_CLK_TCK));

  }
  //std::cout<<"\nUpTime:"<<elapsedTimeSec;
  return elapsedTimeSec;
}