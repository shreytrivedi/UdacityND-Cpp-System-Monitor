#include "linux_system.h"
#include "linux_parser.h"
#include <vector>
#include <iostream>
#include "format.h"

// LinuxSystem::LinuxSystem()
// {
//     mWorker.reset(new std::thread(&LinuxSystem::Run, this));
// }

// LinuxSystem::~LinuxSystem()
// {

// }


float LinuxSystem::MemoryUtilization()
{
    std::lock_guard<std::mutex> lock(mMemUtilMtx);
    return mMemoryUtilization;
}

long LinuxSystem::UpTime()
{
    std::lock_guard<std::mutex> lock(mUpTimeMtx);
    return mUpTime;
}

int LinuxSystem::TotalProcesses()
{
    std::lock_guard<std::mutex> lock(mTotalProcMtx);
    return mTotalProcs;
}

int LinuxSystem::RunningProcesses()
{
    std::lock_guard<std::mutex> lock(mRunningProcMtx);
    return mRunningProcs;
}

std::string LinuxSystem::Kernel()
{
    static std::string kernelName = []()
        {
            return LinuxParser::Kernel();
        }();

    return kernelName;
}

std::string LinuxSystem::OperatingSystem()
{
     static std::string osName = []()
        {
            return LinuxParser::OperatingSystem();
        }();

    return osName;
}


void LinuxSystem::RunSystemTask()
{
    while(true)
    {
        //We will call the individual tasks in order
        UpdateMemoryUtilization();
        UpdateUpTime();
        UpdateTotalProcesses();
        UpdateRunningProcesses();
    }
}

void LinuxSystem::UpdateMemoryUtilization()
{
    auto rp = LinuxParser::MemoryUtilization();
    std::lock_guard<std::mutex> lock(mMemUtilMtx);
    mMemoryUtilization = rp;
}

void LinuxSystem::UpdateUpTime()
{
    auto up = LinuxParser::UpTime();
    std::lock_guard<std::mutex> lock(mUpTimeMtx);
    mUpTime = up;
}

void LinuxSystem::UpdateTotalProcesses()
{
    auto tp = LinuxParser::TotalProcesses();
    std::lock_guard<std::mutex> lock(mTotalProcMtx);
    mTotalProcs = tp;
}

void LinuxSystem::UpdateRunningProcesses()
{
    auto rp = LinuxParser::RunningProcesses();
    std::lock_guard<std::mutex> lock(mRunningProcMtx);
    mRunningProcs = rp;
}

void LinuxSystem::RunProcsTask()
{
    while(true)
    {
        //std::cout<<"\nRunProcsTask:";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::vector<int> pids = LinuxParser::Pids();

        std::vector<Process*> *procs = new std::vector<Process*>;

        for (int pid : pids)
        {
            //Allocate new Process Obj
            Process *proc  = new Process();
            
            //Set Pid
            proc->SetPid(pid);

            //Set User
            proc->SetUser(LinuxParser::User(pid));

            //Set CPU%
            proc->SetCpuUtil(LinuxParser::CpuUtil(pid));

            //Set RAM
            proc->SetRam(LinuxParser::Ram(pid));

            //Set UpTime
            proc->SetUpTime(LinuxParser::UpTime(pid));

            //Set Cmd
            proc->SetComm(LinuxParser::Command(pid));

            procs->push_back(proc);

        }

        //Lets sort by Cpu%

        std::sort((*procs).begin(), (*procs).end(), 
        [](Process* p1, Process* p2)
        {
            return (p1->CpuUtilization() > p2->CpuUtilization());
        });

        {
            std::lock_guard<std::mutex> lk(mProcsMtx);
            std::swap(mProcesses, procs);
        }

        if(!procs)
            continue;

        //clear the procs memory
        for(size_t i = 0; i<procs->size();i++)
        {
            Process* tmp = (*procs)[i];
            delete tmp;
            (*procs)[i] = nullptr;
        }

        procs->clear();

    }
}


Processor& LinuxSystem::Cpu()
{
    return *mCpu;
}

std::vector<Process>& LinuxSystem::Processes()
{
    mReadOnlyProcesses.clear();
    std::lock_guard<std::mutex> lk(mProcsMtx);
    
    if(mProcesses == nullptr)
        return mReadOnlyProcesses;

    for(size_t i = 0; i<mProcesses->size();i++)
    {
        Process* tmp = (*mProcesses)[i];
        mReadOnlyProcesses.push_back(*tmp);
    }

    return mReadOnlyProcesses;
}