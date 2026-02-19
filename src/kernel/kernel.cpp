#include "kernel.h"

Kernel::Kernel(VirtualMachine &machine)
    : machine(machine), errorHandler(machine), loader(machine),
      scheduler(machine) {}

int Kernel::kernelLoadProgram(std::string filePath) {
  std::tuple<int, std::vector<int>> returnTuple;
  int returnCode;
  std::vector<int> asmHeader;

  returnTuple = loader.loadProgram(filePath);
  returnCode = std::get<0>(returnTuple);
  asmHeader = std::get<1>(returnTuple);
  printf("%d\n", returnCode);

  // IF LOAD SUCCEDED THEN CREATE A PCB AND LOAD IT INTO SCHEDULING QUEUES
  if (returnCode == 0) {
    scheduler.initializePcb(asmHeader, filePath);
    return 0;
  }

  // IF LOAD FAILED PUSH ERROR ONTO ERRORLIST
  errorHandler.errorList.push_back(returnCode);
  return 1;
}

int Kernel::kernelRunProgram(std::string filePath) {
  Pcb pcb = scheduler.getPcb(filePath);
  int returnCode;

  returnCode = scheduler.runProgram(pcb);
  printf("returnCode = %d\n", returnCode);

  // IF RUN SUCCEEDS
  if (returnCode == 0) {
    return 0;
  }

  // IF RUN FAILED PUSH ERROR ONTO ERRORLIST
  errorHandler.errorList.push_back(returnCode);
  return 1;
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
