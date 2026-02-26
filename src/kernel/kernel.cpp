#include "kernel.h"

Kernel::Kernel(VirtualMachine &machine)
    : machine(machine), errorHandler(machine), loader(machine),
      scheduler(machine) {}

int Kernel::kernelPrintGanntChart() {
  // HANDLE ERROR LIKE NOT HAVING ANYTHING TO PRINT
  printf("\n[OS][GANNT] \n===========\n");
  for (int i = 0; i < ganntChart.size(); i++) {
    if (std::get<1>(ganntChart[i]) != -1) {
      printf("TIME::[%d] -- PID::[%d]\n", std::get<0>(ganntChart[i]),
             std::get<1>(ganntChart[i]));
    }
  }
  return 0;
}

int Kernel::kernelExecuteProgram(std::multimap<int, std::string> argMap) {
  int returnCode = 0;
  int clockImage = machine.clock;
  ganntChart.clear();

  // DEBUG //
  // for (auto i : argMap)
  //   std::cout << i.first << ": " << i.second << std::endl;

  while (!argMap.empty() || !scheduler.empty()) {

    if (!argMap.empty()) {
      auto key = argMap.begin()->first;
      // printf("[CLOCK] %d\n", machine.clock);

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
    ganntChart.push_back(
        std::make_tuple(machine.clock, scheduler.getCurrentPcbId()));
  }
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

  // IF LOAD SUCCEDED THEN CREATE A PCB AND LOAD IT INTO SCHEDULING QUEUES
  if (returnCode == 0) {
    Pcb *pPcb = scheduler.allocateMemory(asmHeader, filePath);
    scheduler.queuePcb(*pPcb);
    return 0;
  }

  // IF LOAD FAILED PUSH ERROR ONTO ERRORLIST
  errorHandler.errorList.push_back(returnCode);
  return 1;
}

int Kernel::kernelRun() {

  int returnCode = 0;
  while (!scheduler.empty()) {
    returnCode = scheduler.firstComeFirstServe();
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
