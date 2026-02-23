#include "kernel.h"

Kernel::Kernel(VirtualMachine &machine)
    : machine(machine), errorHandler(machine), loader(machine),
      scheduler(machine) {}

int Kernel::kernelExecuteProgram(std::map<int, std::string> argMap) {
  int returnCode = 0;
  int currentClock = machine.clock;
  // fix this
  while (!argMap.empty() | !scheduler.empty()) {
    auto nextProcess = argMap.begin();
    // OFFSET BY LOAD TIME FROM CURRENT CLOCK ON EXECUTION FUNCTION
    if (machine.clock == nextProcess->first + currentClock) {
      returnCode = kernelLoadProgram(nextProcess->second, nextProcess->first);
      if (returnCode == 1) {
        printf("[OS] -- loader failed \n");
      }
      argMap.erase(nextProcess->first);
    }

    scheduler.firstComeFirstServe();
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
    scheduler.allocateMemory(asmHeader, filePath);
    return 0;
  }

  // IF LOAD FAILED PUSH ERROR ONTO ERRORLIST
  errorHandler.errorList.push_back(returnCode);
  return 1;
}

int Kernel::kernelRunSingleProgram(std::string filePath) {

  int returnCode = 0;
  Pcb *pPcb = scheduler.getPcb(filePath);
  printf("pId = %d\n", pPcb->pId);
  printf("pPcb state = %d\n", pPcb->pState);

  while (returnCode != 10) {
    returnCode = scheduler.singleProgram(pPcb);
  }

  // IF RUN FAILED PUSH ERROR ONTO ERRORLIST
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
