#ifndef KERNEL_H
#define KERNEL_H

#include "errorhandler.h"
#include "loader.h"
#include "processLogger.h"
#include "scheduler.h"
#include <chrono>

class Kernel {
public:
  static const u_int8_t NEW = 1;
  static const u_int8_t READY = 2;
  static const u_int8_t RUNNING = 3;
  static const u_int8_t WAITING = 4;
  static const u_int8_t TERMINATED = 5;

  ErrorHandler errorHandler;
  Loader loader;
  Scheduler scheduler;
  ProcessLogger processLogger;
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
  void setVerbosityFlag();

private:
  bool verbosityFlag = false;
};
#endif
