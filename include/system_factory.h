#ifndef SYSTEM_FACTORY_H
#define SYSTEM_FACTORY_H

#include <memory>
#include "system.h"

namespace SystemFactory
{
    std::unique_ptr<System> GetSystem();
}

#endif //SYSTEM_FACTORY_H
