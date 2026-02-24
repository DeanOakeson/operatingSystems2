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
  int kernelLoadProgram(std::string filePath, int arrivalTime = 0);
  int kernelExecuteProgram(std::multimap<int, std::string> argMap);
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

  //////////////
  /// LOGGING //
  //////////////
  int kernelPrintGanntChart();

private:
  std::vector<std::tuple<int, int>> ganntChart;
};
#endif
