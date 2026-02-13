#include "errorhandler.h"
ErrorHandler::ErrorHandler(VirtualMachine &machine) : machine(machine) {}

void ErrorHandler::errorDump() {
  printf("\n[OS] --ERROR_DUMP  \n=================\n");
  for (const int &i : errorList) {
    switch (i) {
    case CPU_EARLY_TERMINATION:
      printf("[OS][100] --cpu early termination\n");
      break;
    case MEM_OVERFLOW:
      printf("[OS][201] --memory overflow--\n");
      break;
    case MEM_OVERWRITE:
      printf("[OS][202] --attempted memory overwrite--\n");
      break;
    case MEM_FSTREAM_ERR_1:
      break;
    case MEM_FSTREAM_ERR_2:
      break;
    case MEM_FSTREAM_ERR_3:
      break;
    case MEM_DUMP_NO_PROGRAM:
      printf("[OS][301] --attempted memDump() no loaded program--\n");
      break;
    }
  }
  printf("\n");
}

void ErrorHandler::coreDump() {
  u_int8_t regId = 0;

  printf("\n[OS] --CORE_DUMP  \n================");
  for (u_int8_t i = 0; i < 6; i++) {
    printf("\nREGISTER:: [ %d ] -- [ %d ]", regId, machine.Reg[regId]);
    regId += 1;
  }
  printf("\nREGISTER:: [ PC ] -- [ %d ] \nREGISTER:: [ Z ] -- "
         "[ %d ]\n\n",
         machine.PC, machine.Z);
}

int ErrorHandler::memDumpAll() {

  // CHECK IF THERE ARE ANY PCBS LOADED IN VMEM
  if (machine.ram.vMemory.size() == 0) { // IF NO FILE HAS BEEN LOADED
    printf("[OS][301] --attempted memDump() with no loaded program\n");
    return 301; // 1 memDump means no load
  }

  for (int i = 0; i < machine.ram.vMemory.size(); i++) {
    printf("\n[OS] --MEM_DUMP  \n===========\nADDRESSES::[ %d - "
           "%d ]\nPROGRAM::[ %s ]\n",
           machine.ram.vMemory[i].fileLoadAddress,
           machine.ram.vMemory[i].fileLoadAddress +
               machine.ram.vMemory[i].fileSize,
           filePath.c_str());
    for (int j = machine.ram.vMemory[i].fileLoadAddress - 1;
         j <= machine.ram.vMemory[i].fileLoadAddress +
                  machine.ram.vMemory[i].fileSize;
         j++) {
      printf("\nADDRESS::[ %d ] -- ", j);
      for (int k = 0; k <= 6; k++) {
        printf("[ %d ]", machine.ram.mem[j][k]);
      }
    }
    printf("\n\n");
  }
  return 0;
}

// FIGURE OUT HOW TO ORGANIZE THE PCBS IN A WAY YOU CAN CALL THIS MANY TIMES.
int ErrorHandler::memDump(std::string filePath) {

  Pcb process; // figure out how to find a pcb from string.

  // CHECK IF THERE ARE ANY PCBS LOADED IN VMEM
  if (machine.ram.vMemory.size() == 0) { // IF NO FILE HAS BEEN LOADED
    printf("[OS][301] --attempted memDump() with no loaded program\n");
    return 301; // 1 memDump means no load
  }

  printf("\n[OS] --MEM_DUMP  \n===========\nADDRESSES::[ %d - "
         "%d ]\nPROGRAM::[ %s ]\n",
         process.fileLoadAddress, process.fileLoadAddress + process.fileSize,
         filePath.c_str());
  for (int j = process.fileLoadAddress - 1;
       j <= process.fileLoadAddress + process.fileSize; j++) {
    printf("\nADDRESS::[ %d ] -- ", j);
    for (int k = 0; k <= 6; k++) {
      printf("[ %d ]", machine.ram.mem[j][k]);
    }
  }
  printf("\n\n");
  return 0;
}
