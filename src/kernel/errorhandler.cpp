#include "errorhandler.h"
ErrorHandler::ErrorHandler(VirtualMachine &machine) : machine(machine) {}

void ErrorHandler::errorDump() {
  std::cout << "[OS]::errh/errorDump\n"
               "==============================================================="
               "==\n";
  for (const int &i : errorList) {
    switch (i) {
    case CPU_EARLY_TERMINATION:
      printf("[OS]::errh -100 --cpu early termination\n");
      break;
    case MEM_OVERFLOW:
      printf("[OS]::errh -201 --memory overflow--\n");
      break;
    case MEM_OVERWRITE:
      printf("[OS]::errh -202 --attempted memory overwrite--\n");
      break;
    case LOAD_FILE_NOT_FOUND:
      printf("[OS]::errh -200 --file not found--\n");
      break;
    case MEM_DUMP_NO_PROGRAM:
      printf("[OS]::errh -301 --attempted memDump() no loaded program--\n");
      break;
    case MEM_DUMP_FALSE_PROGRAM:
      printf("[OS]::errh -302 --attempted memDump() with false program--\n");
      break;
    }
  }
  std::cout
      << "=================================================================\n";
}

void ErrorHandler::coreDump() {
  u_int8_t regId = 0;

  std::cout << "[OS]::errh/coredump\n"
               "========================\n";

  for (u_int8_t i = 0; i < 6; i++) {
    printf("REGISTER:: [ %d ] -- [ %d ]\n", regId, machine.Reg[regId]);
    regId += 1;
  }
  printf("REGISTER:: [ PC ] - [ %d ] \nREGISTER:: [ Z ] -- "
         "[ %d ]\n",
         machine.PC, machine.Z);

  std::cout << "=========================\n";
}

int ErrorHandler::memDumpAll() {

  // CHECK IF THERE ARE ANY PCBS LOADED IN VMEM
  // IF NO FILE HAS BEEN LOADED
  if (machine.ram.vMemory.size() == 0) {
    printf("[OS]errh/mdmp -301 --attempted memDump() with no loaded program\n");
    return 301; // 1 memDump means no load
  }
  printf("\n[OS]errh/memDumpAll \n===========\n");

  // this iterates over the actual vMemory not the hash
  for (int i = 0; i < machine.ram.vMemory.size(); i++) {
    int pFirstInstruction = machine.ram.vMemory[i]->pFirstInstruction;
    int pLoadAddress = machine.ram.vMemory[i]->pLoadAddress;
    int pSize = machine.ram.vMemory[i]->pSize;
    std::string fileName = machine.ram.vMemory[i]->name;

    int fileEnd = pLoadAddress + pSize - 1;

    printf("\nSPACE::[ %d - %d ]\nPROGRAM::[ %s ]\n", pLoadAddress,
           pLoadAddress + pSize, fileName.c_str());

    for (int j = 0; (j + pLoadAddress) <= fileEnd; j++) {
      if (j % 6 == 0) {
        printf("\nADDRESS::[ %d - %d ] -- ", j + pLoadAddress,
               j + pLoadAddress + 6);
      }
      printf("[ %d ]", machine.ram.mem[j + pLoadAddress][0]);
    }
    printf("\n");
  }
  return 0;
}

int ErrorHandler::memDump(std::string filePath) {

  int index;

  try {
    index = machine.ram.vMemoryLookup.at(filePath);
  } catch (std::out_of_range) {
    printf(
        "[OS][ERROR][302] --attempted vMemoryLookup.at() with false program\n");
    return 302;
  }

  Pcb *pPcb = machine.ram.vMemory[index];
  int fileEnd = pPcb->pLoadAddress + pPcb->pSize - 1;

  // CHECK IF THERE ARE ANY PCBS LOADED IN VMEM
  if (machine.ram.vMemory.size() == 0) {
    printf("[OS][ERROR][301] --attempted memDump() with no loaded program\n");
    return 301; // 1 memDump means no load
  }

  printf("\n[OS][ERROR] --MEM_DUMP  \n===========\nSPACE::[ %d - "
         "%d ]\nPROGRAM::[ %s ]\n",
         pPcb->pLoadAddress, pPcb->pLoadAddress + pPcb->pSize,
         filePath.c_str());

  for (int j = 0; (j + pPcb->pLoadAddress) <= fileEnd; j++) {
    if (j % 6 == 0) {
      printf("\nADDRESS::[ %d - %d ] -- ", j + pPcb->pLoadAddress,
             j + pPcb->pLoadAddress + 6);
    }
    printf("[ %d ]", machine.ram.mem[j + pPcb->pLoadAddress][0]);
  }
  return 0;
}
