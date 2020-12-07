#include "linux_processor.h"
#include "linux_parser.h"
#include <chrono>
#include <iostream>

LinuxProcessor::LinuxProcessor()
{
    //std::cout<<"\nLinuxProcessor Created";
    mWorker.reset(new std::thread(&LinuxProcessor::Run, this));
    mPrevActive = mPrevIdle = 0;
}

float LinuxProcessor::Utilization()
{
    //std::cout<<"\nReturning Util:";
    std::lock_guard<std::mutex> guard(mUtilMtx);
    return mUtilization;
}

void LinuxProcessor::Run()
{
    while(true)
    {
        //sleep for 3s
        std::this_thread::sleep_for(std::chrono::seconds(3));

        mCurrIdle = LinuxParser::IdleJiffies();
        mCurrActive = LinuxParser::ActiveJiffies();

        //mCurTotal = LinuxParser::Jiffies();
        mCurTotal = mCurrIdle + mCurrActive;
        mPrevTotal = mPrevIdle + mPrevTotal;

        int diffTotal = abs(mCurTotal - mPrevTotal);
        int diffIdle = abs(mCurrIdle - mPrevIdle);

        float util = float(diffTotal - diffIdle)/(float)diffTotal;
        //std::cout<<"\nUtil:"<<util;
        {
            std::lock_guard<std::mutex> lk(mUtilMtx);
            mUtilization = util;
        }

        //update the prev values;

        mPrevIdle = mCurrIdle;
        mPrevActive = mCurrActive;
    }
}
