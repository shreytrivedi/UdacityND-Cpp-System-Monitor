#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return mPid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return mCpuUtil; }

// TODO: Return the command that generated this process
string Process::Command() { return mComm; }

// TODO: Return this process's memory utilization
string Process::Ram() { return mRam; }

// TODO: Return the user (name) that generated this process
string Process::User() { return mUser; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return mUpTime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// Update: Did not use aas I used a lambda to sort the Procs by CPU%
//bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }