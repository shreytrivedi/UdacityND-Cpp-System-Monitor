#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  //bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  //Setters
  void SetPid(int pid) { mPid = pid; }
  void SetUpTime(long int upTime) { mUpTime = upTime; }
  void SetCpuUtil(float cpuUtil) { mCpuUtil = cpuUtil; }
  void SetUser(std::string user) { mUser = user; }
  void SetComm(std::string comm) { mComm = comm; }
  void SetRam(std::string ram) { mRam = ram; }

  // TODO: Declare any necessary private members
 private:
    
    int mPid;
    long int mUpTime;
    float mCpuUtil;

    std::string mUser;
    std::string mComm;
    std::string mRam;
};

#endif