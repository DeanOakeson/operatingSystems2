#ifndef RAM_H
#define RAM_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_map>

#define MEM_SIZE_KB 12 * 1024

class Ram {
public:
  char mem[MEM_SIZE_KB][6];
  u_int32_t memTable[10][2];

  void loadProgram(std::string FilePath, Ram *pMainMemory) {
    std::ifstream file(FilePath, std::ios::binary);

    // FIRST READ THE FIRST 3 INTS FOR YOUR HEADER VARIABLES //
    u_int32_t fileSize;
    file.read(reinterpret_cast<char *>(&fileSize), sizeof(int));
    printf("fileSize = %d\n", fileSize);
    u_int32_t fileFirstInstruction;
    file.read(reinterpret_cast<char *>(&fileFirstInstruction), sizeof(int));
    printf("fileFirstInstructionOffset = %d\n", fileFirstInstruction);
    u_int32_t fileLoadAddress;
    file.read(reinterpret_cast<char *>(&fileLoadAddress), sizeof(int));
    printf("fileLoaderAddress = %d\n", fileLoadAddress);

    // CREATE ERROR HANDELING WITH VERIFYLOAD//
    this->updateMemoryTable(fileLoadAddress, fileSize);

    // LOAD //
    for (u_int32_t i = fileLoadAddress + fileFirstInstruction;
         i < fileSize + fileLoadAddress; i++) {
      for (u_int32_t j = 0; j < 6; j++) {
        file.read(&this->mem[i][j], sizeof(u_char));
      }
    }
  }

  // PSUEDO MEMORY MANAGMENT//
  void updateMemoryTable(u_int32_t fileLoadAddress, u_int32_t fileSize) {
    if (verifyLoad(fileLoadAddress, fileSize) == false)
      printf("PROGRAM CANNOT BE LOADED IN MEMORY");
    else {
      memTable[1][0] = fileLoadAddress;
      memTable[1][1] = fileSize;
    }
  }

  bool verifyLoad(u_int32_t fileLoadAddress, u_int32_t fileSize) {
    int newLoad = fileLoadAddress;
    int newEnd = fileLoadAddress + fileSize;

    for (int i; i <= 10; i++) {
      int oldLoad = memTable[i][0];
      int oldEnd =
          oldLoad +
          memTable[i][1]; // already loaded program's load position and size
      if (newLoad >= oldLoad && newLoad <= oldEnd)
        return false;
      if (newEnd >= oldLoad && newEnd < +oldEnd)
        return false;
      if (oldEnd >= newLoad && oldEnd <= newEnd)
        return false;
      if (newEnd > MEM_SIZE_KB)
        return false; // MAYBE THROW A DIFFERENT ERROR CODE??
    }
    return true;
  }
};
#endif
