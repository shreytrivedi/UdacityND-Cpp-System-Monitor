#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    int hrs=0;
    int mins=0;
    int secs=0;

    if(seconds>=60)
    {
        mins = seconds/60;
        secs = seconds%60;
    }

    if(mins>=60)
    {
        hrs = mins / 60;
        mins = mins % 60;
    }

    char buff[20]={0};
    snprintf(buff, sizeof(buff), "%02d:%02d:%02d", hrs, mins, secs);
    string time{buff};
    return time;
}