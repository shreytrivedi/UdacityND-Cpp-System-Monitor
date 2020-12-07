#include "ncurses_display.h"
#include "system.h"
#include "system_factory.h"


int main() {
  //std::unique_ptr<System> system = SystemFactory::GetSystem();
  System *system = SystemFactory::GetSystem();
  NCursesDisplay::Display(*system);
}