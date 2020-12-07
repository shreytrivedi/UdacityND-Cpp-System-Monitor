
#include "system_factory.h"
#include "linux_system.h"

//Include other OS Systems as they are created

//Temporary should goto into build setting eventually
#define LINUX_SYSTEM 1;

namespace SystemFactory
{
    std::unique_ptr<System> GetSystem()
    {
#ifdef LINUX_SYSTEM
        //return std::make_unique<LinuxSystem>();
        std::unique_ptr<System> obj(new LinuxSystem());
        return std::move(obj);
#endif
    }

    // System* GetSystem()
    // {
    //     return new LinuxSystem();
    // }
}