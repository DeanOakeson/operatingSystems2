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
    case MEM_FSTREAM_ERR_1:
      break;
    case MEM_FSTREAM_ERR_2:
      break;
    case MEM_FSTREAM_ERR_3:
      break;
    case MEM_DUMP_NO_PROGRAM:
      printf("[OS][ERROR][301] --attempted memDump() no loaded program--\n");
      break;
    }
  }
  printf("\n");
}

void ErrorHandler::coreDump() {
  u_int8_t regId = 0;

  printf("\n[OS][ERROR] --CORE_DUMP  \n================");
  for (u_int8_t i = 0; i < 6; i++) {
    printf("\nREGISTER:: [ %d ] -- [ %d ]", regId, machine.Reg[regId]);
    regId += 1;
  }
  printf("\nREGISTER:: [ PC ] -- [ %d ] \nREGISTER:: [ Z ] -- "
         "[ %d ]\n\n",
         machine.PC, machine.Z);
}

int ErrorHandler::memDumpAll() {

  // NEEDS TO BE COMPLETLEY IMPLPLEMENTED

  // CHECK IF THERE ARE ANY PCBS LOADED IN VMEM
  if (machine.ram.vMemory.size() == 0) { // IF NO FILE HAS BEEN LOADED
    printf("[OS][ERROR][301] --attempted memDump() with no loaded program\n");
    return 301; // 1 memDump means no load
  }

  for (int i = 0; i < machine.ram.vMemory.size(); i++) {
    int fileFirstInstruction = machine.ram.vMemory[i].fileFirstInstruction;
    int fileLoadAddress = machine.ram.vMemory[i].fileLoadAddress;
    int fileSize = machine.ram.vMemory[i].fileSize;

    int fileEnd = fileLoadAddress + fileSize - 1;

    printf("\n[OS][ERROR] --MEM_DUMP  \n===========\nADDRESSES::[ %d - "
           "%d ]\nPROGRAM::[ %s ]\n",
           fileLoadAddress, fileLoadAddress + fileSize, filePath.c_str());

    for (int j = 0; (j + fileLoadAddress + fileFirstInstruction) <= fileEnd;
         j++) {
      if (j % 6 == 0) {
        printf("\nADDRESS::[ %d - %d ] -- ", j + fileLoadAddress,
               j + fileLoadAddress + 5);
      }
      printf("[ %d ]",
             machine.ram.mem[j + fileLoadAddress + fileFirstInstruction][0]);
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
    printf("[OS][ERROR][301] --attempted memDump() with no loaded program\n");
    return 301; // 1 memDump means no load
  }

  printf("\n[OS][ERROR] --MEM_DUMP  \n===========\nADDRESSES::[ %d - "
         "%d ]\nPROGRAM::[ %s ]\n",
         process.fileLoadAddress, process.fileLoadAddress + process.fileSize,
         filePath.c_str());
  for (int j = process.fileLoadAddress - 1;
       j <= process.fileLoadAddress + process.fileSize / 6; j++) {
    if (j % 6 == 0) {
      printf("\nADDRESS::[ %d ] -- ", j);
    }
    printf("[ %d ]", machine.ram.mem[j][0]);
  }
  printf("\n\n");
  return 0;
}
