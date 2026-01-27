#ifndef KERNEL_H
#define KERNEL_H

#include "ram.h"
#include "vm.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdio.h>
#include <stdlib.h>

class Kernel {
public:
  VirtualMachine Machine;
  std::string filePath;

  void initHardware(VirtualMachine newMachine) { Machine = newMachine; }

  //////////////////////
  // CPU SYSTEM CALLS //
  //////////////////////

  void coreDump() {
    u_int8_t regId = 0;

    printf("\nCORE_DUMP  \n=========");
    for (u_int8_t i = 0; i < 6; i++) {
      printf("\nREGISTER:: [ %d ] -- [ %d ]", regId, Machine.reg[regId]);
      regId += 1;
    }
    printf("\nREGISTER:: [ PC ] -- [ %d ] \nREGISTER:: [ Z ] -- [ %d ]\n\n",
           Machine.PC, Machine.Z);
  }

  void runProgram() { Machine.runCpu(); }

  //////////////////////
  // MEM SYSTEM CALLS //
  //////////////////////

  int loadProgram(std::string FilePath) {
    std::ifstream file(FilePath, std::ios::binary);

    // FIRST READ THE FIRST 3 INTS FOR HEADER VARIABLES //
    file.read(reinterpret_cast<char *>(&Machine.MainMemory.fileSize),
              sizeof(u_int32_t));
    file.read(
        reinterpret_cast<char *>(&Machine.MainMemory.fileFirstInstruction),
        sizeof(u_int32_t));
    file.read(reinterpret_cast<char *>(&Machine.MainMemory.fileLoadAddress),
              sizeof(u_int32_t));

    // TRANSLATE INTO 6 BYTE ADDRESS SPACE
    Machine.MainMemory.fileSize = Machine.MainMemory.fileSize / 6;

    // FILE SIZE EXCEEDS MEMORY
    if (Machine.MainMemory.fileLoadAddress + Machine.MainMemory.fileSize >
        MEM_SIZE_KB) {
      return 2;
    }

    // MEMORY IS OCCUPIED
    if (verifyMemoryIsUnoccupied() != true) {
      return 1;
    }

    for (u_int32_t i = Machine.MainMemory.fileLoadAddress;
         i < Machine.MainMemory.fileSize + Machine.MainMemory.fileLoadAddress;
         i++) {
      for (u_int32_t j = 0; j < 6; j++) {
        file.read(reinterpret_cast<char *>(&Machine.MainMemory.mem[i][j]),
                  sizeof(unsigned char));
      }
    }

    updateMemoryIndicators();

    filePath = FilePath;
    return 0;
  }

  int memDump() {
    if (Machine.MainMemory.fileSize == 0) { // IF NO FILE HAS BEEN LOADED
      printf("\n --NO LOADED PROGRAM-- \n");
    } // IF SOMETHING HAS BEEN LOADED
    printf("\nERROR_DUMP  \n===========\nADDRESSES::[ %d - "
           "%d ]\nPROGRAM::[ %s ]\n",
           Machine.MainMemory.fileLoadAddress,
           Machine.MainMemory.fileLoadAddress + Machine.MainMemory.fileSize,
           filePath.c_str());
    for (int i = Machine.MainMemory.fileLoadAddress;
         i < Machine.MainMemory.fileLoadAddress + Machine.MainMemory.fileSize;
         i++) {
      printf("\nADDRESS::[ %d ] -- ", i);
      for (int j = 0; j <= 6; j++) {
        printf("[ %d ]", Machine.MainMemory.mem[i][j]);
      }
    }
    printf("\n\n");
    return 0;
  }

private:
  /////////////////////////////
  // MEMORY HELPER FUNCTIONS //
  /////////////////////////////

  // CHECKS ALL POSITIONS FOR THE TO BE LOADED PROGRAM //
  bool verifyMemoryIsUnoccupied() {
    for (u_char i = Machine.MainMemory.fileLoadAddress;
         i <= Machine.MainMemory.fileLoadAddress + Machine.MainMemory.fileSize;
         i++) {
      if (Machine.MainMemory.mem[i][6] == 1) {
        return false;
      }
    }
    return true;
  }

  // FLIPS THE MEMORY OCCUPATION BIT //
  void updateMemoryIndicators() {
    for (u_int32_t i = Machine.MainMemory.fileLoadAddress;
         i <= Machine.MainMemory.fileLoadAddress + Machine.MainMemory.fileSize;
         i++) {
      if (Machine.MainMemory.mem[i][6] == 0)
        Machine.MainMemory.mem[i][6] = 1;
      else if (Machine.MainMemory.mem[i][6] == 1)
        Machine.MainMemory.mem[i][6] = 0;
    }
  }
};

#endif
