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
  std::vector<int> errorList = {};

  /////////////////////
  // ERROR HANDELING //
  /////////////////////

  static const u_int32_t CPU_EARLY_TERMINATION = 100;
  static const u_int32_t MEM_OVERFLOW = 201;
  static const u_int32_t MEM_OVERWRITE = 202;
  static const u_int32_t MEM_FSTREAM_ERR_1 = 203;
  static const u_int32_t MEM_FSTREAM_ERR_2 = 204;
  static const u_int32_t MEM_FSTREAM_ERR_3 = 205;
  static const u_int32_t MEM_DUMP_NO_PROGRAM = 301;

  void errorDump() {
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

  void initHardware(VirtualMachine newMachine) { Machine = newMachine; }

  //////////////////////
  // CPU SYSTEM CALLS //
  //////////////////////
  ///
  /// CPU ERRORS 100 - 200

  void coreDump() {
    u_int8_t regId = 0;

    printf("\n[OS] --CORE_DUMP  \n================");
    for (u_int8_t i = 0; i < 6; i++) {
      printf("\nREGISTER:: [ %d ] -- [ %d ]", regId, Machine.reg[regId]);
      regId += 1;
    }
    printf("\nREGISTER:: [ PC ] -- [ %d ] \nREGISTER:: [ Z ] -- "
           "[ %d ]\n\n",
           Machine.PC, Machine.Z);
  }

  int runProgram() {
    int returnCode = Machine.runCpu();
    if (returnCode == 100) {
      errorList.push_back(10);
      return 100;
    }
    return 0;
  }

  //////////////////////
  // MEM SYSTEM CALLS //
  //////////////////////
  ///
  /// MEM ERRORS 200 -- 300

  int loadProgram(std::string FilePath) {

    std::ifstream file(FilePath, std::ios::binary);

    // //
    // FIGURE THESE ERRORS OUT
    // FIGURE OUT HOW TO LOAD WORDS AT THE TOP OF TH ASSEMBLY
    // //

    if (file.fail()) {
      std::ios_base::iostate state = file.rdstate();

      if (state & std::ios_base::eofbit) {
        errorList.push_back(MEM_FSTREAM_ERR_1);
        return MEM_FSTREAM_ERR_1; // END OF FILE REACHED FAIL
      }
      if (state & std::ios_base::failbit) {
        errorList.push_back(MEM_FSTREAM_ERR_2);
        return MEM_FSTREAM_ERR_2; // NON_FATAL I/O ERROR OCCURED
      }
      if (state & std::ios_base::failbit) {
        errorList.push_back(MEM_FSTREAM_ERR_3);
        return MEM_FSTREAM_ERR_3; // FATAL ERROR OCCURED
      }
    }

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

    // MEMORY OVERFLOW
    if (Machine.MainMemory.fileLoadAddress + Machine.MainMemory.fileSize >
        MEM_SIZE_KB) {
      printf("[OS][201] --memory overflow, load "
             "cancelled\n");
      errorList.push_back(MEM_OVERFLOW);
      return MEM_OVERFLOW;
    }

    // MEMORY IS OCCUPIED
    if (verifyMemoryIsUnoccupied() != true) {
      errorList.push_back(MEM_OVERWRITE);
      printf("[OS][202] --attempted to overwrite existing memory, load "
             "cancelled\n");
      return MEM_OVERWRITE;
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
      printf("[OS][301] --attempted memDump() with no loaded program\n");
      errorList.push_back(MEM_DUMP_NO_PROGRAM);
      return 301; // 1 memDump means no load
    }
    printf("\n[OS] --MEM_DUMP  \n===========\nADDRESSES::[ %d - "
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
    for (int i = Machine.MainMemory.fileLoadAddress;
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
    for (int i = Machine.MainMemory.fileLoadAddress;
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
