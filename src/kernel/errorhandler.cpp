#include "errorhandler.h"
ErrorHandler::ErrorHandler(VirtualMachine &machine) : machine(machine) {}

void ErrorHandler::errorDump() {
  printf("\n[OS] --ERROR_DUMP  \n=================\n");
  for (const int &i : errorList) {
    switch (i) {
    case CPU_EARLY_TERMINATION:
      printf("[OS][ERROR][100] --cpu early termination\n");
      break;
    case MEM_OVERFLOW:
      printf("[OS][ERROR][201] --memory overflow--\n");
      break;
    case MEM_OVERWRITE:
      printf("[OS][ERROR][202] --attempted memory overwrite--\n");
      break;
    case LOAD_FILE_NOT_FOUND:
      printf("[OS][ERROR][200] --file not found--\n");
      break;
    case MEM_DUMP_NO_PROGRAM:
      printf("[OS][ERROR][301] --attempted memDump() no loaded program--\n");
      break;
    case MEM_DUMP_FALSE_PROGRAM:
      printf("[OS][ERROR][302] --attempted memDump() with false program--\n");
      break;
    }
  }
}

void ErrorHandler::coreDump() {
  u_int8_t regId = 0;

  printf("\n[OS][ERROR] --CORE_DUMP  \n================");
  for (u_int8_t i = 0; i < 6; i++) {
    printf("\nREGISTER:: [ %d ] -- [ %d ]", regId, machine.Reg[regId]);
    regId += 1;
  }
  printf("\nREGISTER:: [ PC ] -- [ %d ] \nREGISTER:: [ Z ] -- "
         "[ %d ]\n",
         machine.PC, machine.Z);
}

int ErrorHandler::memDumpAll() {

  // CHECK IF THERE ARE ANY PCBS LOADED IN VMEM
  // IF NO FILE HAS BEEN LOADED
  if (machine.ram.vMemory.size() == 0) {
    printf("[OS][ERROR][301] --attempted memDump() with no loaded program\n");
    return 301; // 1 memDump means no load
  }
  printf("\n[OS][ERROR] --MEM_DUMP  \n===========\n");

  // this iterates over the actual vMemory not the hash
  for (int i = 0; i < machine.ram.vMemory.size(); i++) {
    int pFirstInstruction = machine.ram.vMemory[i].pFirstInstruction;
    int pLoadAddress = machine.ram.vMemory[i].pLoadAddress;
    int pSize = machine.ram.vMemory[i].pSize;
    std::string fileName = machine.ram.vMemory[i].name;

    int fileEnd = pLoadAddress + pSize - 1;

    printf("\nSPACE::[ %d - %d ]\nPROGRAM::[ %s ]\n", pLoadAddress,
           pLoadAddress + pSize, fileName.c_str());

    for (int j = 0; (j + pLoadAddress + pFirstInstruction) <= fileEnd; j++) {
      if (j % 6 == 0) {
        printf("\nADDRESS::[ %d - %d ] -- ", j + pLoadAddress,
               j + pLoadAddress + 5);
      }
      printf("[ %d ]",
             machine.ram.mem[j + pLoadAddress + pFirstInstruction][0]);
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

  Pcb process = machine.ram.vMemory[index];
  int fileEnd = process.pLoadAddress + process.pSize - 1;

  // CHECK IF THERE ARE ANY PCBS LOADED IN VMEM
  if (machine.ram.vMemory.size() == 0) {
    printf("[OS][ERROR][301] --attempted memDump() with no loaded program\n");
    return 301; // 1 memDump means no load
  }

  printf("\n[OS][ERROR] --MEM_DUMP  \n===========\nSPACE::[ %d - "
         "%d ]\nPROGRAM::[ %s ]\n",
         process.pLoadAddress, process.pLoadAddress + process.pSize,
         filePath.c_str());

  for (int j = 0;
       (j + process.pLoadAddress + process.pFirstInstruction) <= fileEnd; j++) {
    if (j % 6 == 0) {
      printf("\nADDRESS::[ %d - %d ] -- ", j + process.pLoadAddress,
             j + process.pLoadAddress + 5);
    }
    printf("[ %d ]",
           machine.ram
               .mem[j + process.pLoadAddress + process.pFirstInstruction][0]);
  }
  return 0;
}
