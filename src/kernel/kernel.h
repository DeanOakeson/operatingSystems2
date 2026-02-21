#ifndef KERNEL_H
#define KERNEL_H

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

  /////////////////////////
  // PROGRAM INTERACTION //
  /////////////////////////
  int kernelLoadProgram(std::string filePath);
  int kernelRunSingleProgram(std::string filePath);
  int kernelRun();

  ////////////////////////
  // MEMORY INTERACTION //
  ////////////////////////
  int kernelMemDump(std::string filePath);
  int kernelErrorDump();
  int kernelMemDumpAll();

  /////////////////////
  // CPU INTERACTION //
  /////////////////////
  int kernelCoreDump();
};
#endif
