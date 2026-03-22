#ifndef ERROR_H
#define ERROR_H

#include "../hardware/vm.h"
#include "processlog.h"
#include <iostream>
#include <queue>

static const u_int32_t CPU_EARLY_TERMINATION = 100;
static const u_int32_t LOAD_FILE_NOT_FOUND = 200;
static const u_int32_t MEM_OVERFLOW = 201;
static const u_int32_t MEM_OVERWRITE = 202;
static const u_int32_t MEM_DUMP_NO_PROGRAM = 301;
static const u_int32_t MEM_DUMP_FALSE_PROGRAM = 302;
static const u_int32_t CORE_DUMP_NO_PRCLOG = 303;

class ErrorHandler {
public:
  VirtualMachine &machine;
  std::string filePath = "";
  std::vector<int> errorList = {};

  ErrorHandler(VirtualMachine &machine);
  void setVerbosityFlag();
  void logTerminatedProcesses(std::queue<Pcb *> &terminatedQueue);
  ProcessLog *getCpuLog(int index);

  int errorDump();
  void coreDump(ProcessLog &processLog);
  int memDumpAll();
  int memDump(std::string filePath);
  void printGannt(Pcb &process, int globalStart, int globalEnd);

private:
  int cpuLogCount = 0;
  ;
  bool verbosityFlag;
  std::vector<ProcessLog *> cpuLogs = {};
};
#endif
