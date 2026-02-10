#ifndef KERNEL_H
#define KERNEL_H

#include "../hardware/vm.h"
#include "errorhandler.h"
#include "loader.h"
#include "scheduler.h"

class Kernel {
public:
  ErrorHandler errorHandler;
  Loader loader;
  Scheduler scheduler;
  VirtualMachine &machine;

  Kernel(VirtualMachine &machine);
};
#endif
