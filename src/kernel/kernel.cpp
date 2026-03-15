#include "kernel.h"

Kernel::Kernel(VirtualMachine &machine)
    : machine(machine), errorHandler(machine), loader(machine),
      scheduler(machine) {}

int Kernel::kernelPrintGanntChart() {
  // HANDLE ERROR LIKE NOT HAVING ANYTHING TO PRINT

  printf("\n[OS][GANNT] \n===========\n");
  processLogger.printGannt();
  return 0;
}

void Kernel::setVerbosityFlag() {
  if (verbosityFlag == true) {
    verbosityFlag = false;
    scheduler.setVerbosityFlag();
    processLogger.setVerbosityFlag();
    loader.setVerbosityFlag();
    return;
  }

  verbosityFlag = true;
  scheduler.setVerbosityFlag();
  processLogger.setVerbosityFlag();
  loader.setVerbosityFlag();
}

int Kernel::kernelExecuteProgram(std::multimap<int, std::string> argMap) {
  auto start = std::chrono::high_resolution_clock::now();
  int returnCode;
  int clockImage = machine.clock;

  while (!argMap.empty() || !scheduler.empty()) {

    if (!argMap.empty()) {
      auto key = argMap.begin()->first;
      //  LOAD ALL ELEMENTS THAT ARRIVE AT A SPECIFIC TIME
      if (argMap.begin()->first + clockImage == machine.clock) {
        auto range = argMap.equal_range(key);
        for (auto i = range.first; i != range.second; ++i) {
          returnCode = kernelLoadProgram(i->second, i->first);
        }
        argMap.erase(key);
      }
    }
    scheduler.firstComeFirstServe();
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  if (verbosityFlag == true) {
    std::cout << "[OS]::exec --cpu runtime = ";
    std::cout << duration;
    std::cout << " m/s \n";
  }

  while (!scheduler.terminatedQueue.empty()) {
    processLogger.logProcess(*scheduler.popQueue(TERMINATED));
  }

  // gannt = scheduler.ganntDataDump();
  // error = scheduler.errorDataDump();
  return 0;
}

int Kernel::kernelLoadProgram(std::string filePath, int arrivalTime) {
  std::tuple<int, std::vector<int>> returnTuple;
  int returnCode;
  std::vector<int> asmHeader;

  returnTuple = loader.loadProgram(filePath);
  returnCode = std::get<0>(returnTuple);
  asmHeader = std::get<1>(returnTuple);

  asmHeader.push_back(arrivalTime);

  // IF LOAD SUCCEDED THEN CREATE A PCB ALLOCATE MEMORY AND LOAD IT INTO
  // SCHEDULING QUEUES
  if (returnCode == 0) {
    Pcb *pPcb = scheduler.createPcb(asmHeader, filePath);
    scheduler.allocateMemory(*pPcb);
    scheduler.queuePcb(*pPcb, 1);
    return 0;
  }

  // IF LOAD FAILED PUSH ERROR ONTO ERRORLIST
  errorHandler.errorList.push_back(returnCode);
  return 1;
}

int Kernel::kernelRun() {
  int returnCode;
  while (!scheduler.empty()) {
    returnCode = scheduler.firstComeFirstServe();
  }
  while (!scheduler.terminatedQueue.empty()) {
    processLogger.logProcess(*scheduler.popQueue(TERMINATED));
  }
  return 0;
}

int Kernel::kernelMemDump(std::string filePath) {
  int returnCode;
  returnCode = errorHandler.memDump(filePath);

  // IF MEMDUMP SUCCEEDS RETURN 0;
  if (returnCode == 0) {
    return 0;
  }
  // IF MEMDUMP FAILS PUSH ERROR ONTO LIST
  errorHandler.errorList.push_back(returnCode);
  return 1;
}

int Kernel::kernelMemDumpAll() {
  int returnCode;
  returnCode = errorHandler.memDumpAll();

  // IF MEMDUMPALL SUCCEEDS RETURN 0;
  if (returnCode == 0) {
    return 0;
  }
  // IF MEMDUMP FAILS PUSH ERROR ONTO LIST
  errorHandler.errorList.push_back(returnCode);
  return 1;
}

int Kernel::kernelCoreDump() {
  errorHandler.coreDump();
  return 0;
}

int Kernel::kernelErrorDump() {
  errorHandler.errorDump();
  return 0;
}
