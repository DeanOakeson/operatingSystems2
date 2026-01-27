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
  unsigned char mem[MEM_SIZE_KB][7];

  // CREATE VMEM //
  u_int32_t fileSize = 0;
  u_int32_t fileFirstInstruction = 0;
  u_int32_t fileLoadAddress = 0;
};
#endif
