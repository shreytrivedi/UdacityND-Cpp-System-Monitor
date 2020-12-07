#ifndef LINUX_PROCESSOR_H
#define LINUX_PROCESSOR_H

#include "processor.h"

#include <mutex>
#include <thread>

class LinuxProcessor : public Processor
{
public:

    LinuxProcessor();

    virtual ~LinuxProcessor()
    {

    }

    virtual float Utilization() override;

private:

    void Run();

    float mUtilization;
    std::mutex mUtilMtx;
    std::unique_ptr<std::thread> mWorker;

    long mPrevIdle;
    long mPrevActive;

    long mCurrIdle;
    long mCurrActive;

    long mCurTotal;
    long mPrevTotal;


};

#endif //LINUX_PROCESSOR_H