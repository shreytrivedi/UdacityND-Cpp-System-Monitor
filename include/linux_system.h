#ifndef LINUX_SYSTEM_H
#define LINUX_SYSTEM_H

#include "system.h"
#include "linux_parser.h"

#include <mutex>
#include <thread>
#include "linux_processor.h"

class LinuxSystem : public System
{
public:

    LinuxSystem()
    {
        mSystemWorker.reset(new std::thread(&LinuxSystem::RunSystemTask, this));
        mProcWorker.reset(new std::thread(&LinuxSystem::RunProcsTask, this));
        mCpu.reset(new LinuxProcessor());
    }

    virtual ~LinuxSystem()
    {

    }

    //Implementation from System

    virtual Processor& Cpu() override;        
    virtual std::vector<Process>& Processes() override;  
    virtual float MemoryUtilization() override;          
    virtual long UpTime() override;                      
    virtual int TotalProcesses() override;               
    virtual int RunningProcesses() override;             
    virtual std::string Kernel() override;               
    virtual std::string OperatingSystem() override;      

private:

    //Worker Thread's Runner
    void RunSystemTask();
    void RunProcsTask();

    //Worker's System SubTasks
    void UpdateMemoryUtilization();
    void UpdateUpTime();
    void UpdateTotalProcesses();
    void UpdateRunningProcesses();
    
    long mUpTime;
    int mTotalProcs;
    int mRunningProcs;
    float mMemoryUtilization;

    std::mutex mUpTimeMtx;
    std::mutex mTotalProcMtx;
    std::mutex mRunningProcMtx;
    std::mutex mMemUtilMtx;
    std::mutex mProcsMtx;

    std::unique_ptr<std::thread> mSystemWorker;
    std::unique_ptr<std::thread> mProcWorker;

    std::unique_ptr<Processor> mCpu;
    std::vector<Process*> *mProcesses;
    std::vector<Process> mReadOnlyProcesses;
};


#endif //LINUX_SYSTEM_H