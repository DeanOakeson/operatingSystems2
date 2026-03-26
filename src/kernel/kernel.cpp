#include "kernel.h"

Kernel::Kernel(VirtualMachine &machine)
    : machine(machine), errorHandler(machine), loader(machine),
      scheduler(machine) {}

int Kernel::kernelPrintGanntChart() {
  // HANDLE ERROR LIKE NOT HAVING ANYTHING TO PRINT

  printf("\n[OS][GANNT] \n===========\n");
  // processLogger.printGannt();
  return 0;
}

void Kernel::setVerbosityFlag() {
  if (verbosityFlag == true) {
    verbosityFlag = false;
    scheduler.setVerbosityFlag();
    errorHandler.setVerbosityFlag();
    loader.setVerbosityFlag();
    return;
  }

  verbosityFlag = true;
  scheduler.setVerbosityFlag();
  errorHandler.setVerbosityFlag();
  loader.setVerbosityFlag();
}

std::multimap<int, std::string>
Kernel::kernelExecuteProgram(std::multimap<int, std::string> argMap) {
  std::multimap<int, std::string> returnMap;
  auto start = std::chrono::high_resolution_clock::now();
  int returnCode;
  int clockImage = machine.clock;

  while (!argMap.empty() || !scheduler.allQueuesEmpty()) {
    // Drain all programs whose arrival time has been reached
    while (!argMap.empty() &&
           argMap.begin()->first + clockImage <= machine.clock) {
      auto arrival = argMap.begin()->first;
      auto range = argMap.equal_range(arrival);
      for (auto i = range.first; i != range.second; ++i) {
        returnCode = kernelLoadProgram(i->second, i->first);
        returnMap.insert({returnCode, i->second});
      }
      argMap.erase(arrival);
    }

    switch (schedulerAlgo) {
    case (FCFS):
      scheduler.firstComeFirstServe();
    case (RR):
      scheduler.roundRobin(schedulerQuantum);
    case (MLFQ):
      scheduler.multiLevelFeedbackQueue(schedulerQuantum, mlfqRatio);
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();
  if (verbosityFlag == true) {
    std::cout << "[OS]::exec - cpu runtime = " << duration << "m/s \n";
  }
  if (!scheduler.terminatedQueue.empty()) {
    // push completion time onto process log
    errorHandler.logTerminatedProcesses(scheduler.terminatedQueue);
  }
  return returnMap;
}

int Kernel::kernelLoadProgram(std::string filePath, int arrivalTime) {
  std::tuple<int, std::vector<int>> returnTuple;
  int returnCode;
  std::vector<int> asmHeader;

  returnTuple = loader.loadProgram(filePath);
  returnCode = std::get<0>(returnTuple);
  asmHeader = std::get<1>(returnTuple);
  asmHeader.push_back(machine.clock);

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
  while (!scheduler.allQueuesEmpty()) {
    returnCode = scheduler.roundRobin(3);
  }
  if (!scheduler.terminatedQueue.empty()) {
    errorHandler.logTerminatedProcesses(scheduler.terminatedQueue);
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

int Kernel::kernelCoreDump(int index) {
  ProcessLog *pPrcLog = errorHandler.getCpuLog(index);
  if (pPrcLog != NULL) {
    errorHandler.coreDump(*pPrcLog);
    return 0;
  }
  errorHandler.errorList.push_back(303);
  return 1;
}

int Kernel::kernelErrorDump() {
  int returnCode = errorHandler.errorDump();
  return returnCode;
}
