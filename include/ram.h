#ifndef RAM_H
#define RAM_H

#include <fstream>
#include <iostream>
#include <string.h>
#include <unordered_map>

#define MEM_SIZE_KB 512 * 1024 // 512 KB

class Ram {
public:
  // OBJECTS//
  unsigned char mem[MEM_SIZE_KB][7];

  // CREATE VMEM //
  int fileSize = 0;
  int fileFirstInstruction = 0;
  int fileLoadAddress = 0;
};
#endif
