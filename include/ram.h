#ifndef RAM_H
#define RAM_H

#include <fstream>
#include <iostream>
#include <string.h>
#include <unordered_map>

#define MEM_SIZE_KB 12 * 1024

class Ram {
public:
  // OBJECTS//
  char mem[MEM_SIZE_KB][7];
  std::string filePath;
  u_int32_t fileSize = 0;
  u_int32_t fileFirstInstruction = 0;
  u_int32_t fileLoadAddress = 0;

  // loadProgram returns 0 on success, 1 on memory override, and 2 on memory
  // overflow
  int loadProgram(std::string FilePath) {
    std::ifstream file(FilePath, std::ios::binary);

    // FIRST READ THE FIRST 3 INTS FOR YOUR HEADER VARIABLES //
    file.read(reinterpret_cast<char *>(&fileSize), sizeof(int));
    file.read(reinterpret_cast<char *>(&fileFirstInstruction), sizeof(int));
    file.read(reinterpret_cast<char *>(&fileLoadAddress), sizeof(int));

    if (fileLoadAddress + fileSize > MEM_SIZE_KB) {
      return 2;
    }

    bool memoryIsUnoccupied = this->verifyMemoryIsUnoccupied();

    if (memoryIsUnoccupied == true) {
      for (u_int32_t i = fileLoadAddress + fileFirstInstruction;
           i < fileSize + fileLoadAddress; i++) {
        for (u_int32_t j = 0; j < 6; j++) {
          file.read(&this->mem[i][j], sizeof(u_char));
        }
      }
      updateMemoryIndicators();

      filePath =
          FilePath; // THIS CHANGES THE MOST RECENT FILEPATH LOADED FOR PRINTING
    } else {
      return 1; // Failure due to memory override
    }
    return 0; // Returns successfully
  }

  int memDump() {
    if (fileSize == 0) { // IF NO FILE HAS BEEN LOADED
      printf("\n --NO LOADED PROGRAM-- \n");
    } else { // IF SOMETHING HAS BEEN LOADED
      printf("\nMEMORY ADDRESSES %d --- %d\n PROGRAM: %s", fileLoadAddress,
             fileLoadAddress + fileSize, filePath.c_str());
      for (int i = fileLoadAddress; i < fileLoadAddress + fileSize; i++) {
        printf("\n-- addr::%d -- ", i);
        for (int j = 0; j <= 6; j++) {
          printf("%d ", mem[i][j]);
        }
      }
    };

    printf("\n\n");
    return 0;
  }

private:
  // CHECKS ALL POSITIONS THE POTENTIAL PROGRAM WOULD OCCUPY TO MAKE SURE THAT
  // IT IS FREE, returns false if occupied
  bool verifyMemoryIsUnoccupied() {
    for (u_int32_t i = fileLoadAddress; i <= fileLoadAddress + fileSize; i++) {
      if (mem[i][6] == 1) {
        return false;
      }
    }
    return true;
  }

  // FLIPS THE MEMORY OCCUPATION BIT no return
  void updateMemoryIndicators() {
    for (u_int32_t i = fileLoadAddress; i <= fileLoadAddress + fileSize; i++) {
      if (mem[i][6] == 0)
        mem[i][6] = 1;
      else if (mem[i][6] == 1)
        mem[i][6] = 0;
    }
  }
};
#endif
