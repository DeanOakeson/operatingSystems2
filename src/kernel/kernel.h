#ifndef KERNEL_H
#define KERNEL_H

#include "errorhandler.h"
#include "loader.h"
#include "scheduler.h"
#include <chrono>

class Kernel {
public:
  static const u_int8_t NEW = 1;
  static const u_int8_t READY = 2;
  static const u_int8_t RUNNING = 3;
  static const u_int8_t WAITING = 4;
  static const u_int8_t TERMINATED = 5;

  static const u_int8_t FCFS = 6;
  static const u_int8_t RR = 7;
  static const u_int8_t MLFQ = 8;

  int mlfqRatio = 5;
  int schedulerQuantum = 3;

  ErrorHandler errorHandler;
  Loader loader;
  Scheduler scheduler;
  VirtualMachine &machine;

  Kernel(VirtualMachine &machine);

  /////////////////////////
  // PROGRAM INTERACTION //
  /////////////////////////
  int kernelLoadProgram(std::string filePath, int arrivalTime = 0);
  std::multimap<int, std::string>
  kernelExecuteProgram(std::multimap<int, std::string> argMap);
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
  int kernelCoreDump(int index = -1);

  //////////////
  /// LOGGING //
  //////////////
  int kernelPrintGanntChart();
  void setVerbosityFlag();

private:
  bool verbosityFlag = false;
  int schedulerAlgo = MLFQ;
};
#endif
