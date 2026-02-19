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

  int kernelLoadProgram(std::string filePath);
  int kernelRunProgram(std::string filePath);
  int kernelMemDump(std::string filePath);
  int kernelErrorDump();
  int kernelMemDumpAll();
  int kernelCoreDump();
};
#endif
