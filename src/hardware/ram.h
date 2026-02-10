#include "pcb.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <unordered_map>

#define MEM_SIZE_KB 12 * 1024 // 512 KB

class Ram {
public:
  // OBJECTS//
  unsigned char mem[MEM_SIZE_KB][7];

  // CREATE VMEM //
  std::vector<Pcb> vMemory;

  int initializePcb(std::vector<int> asmHeader, std::string filePath) {
    Pcb newPcb;
    newPcb.fileLoadAddress = asmHeader[0];
    newPcb.fileEndAddress = asmHeader[0] + (asmHeader[1]);
    newPcb.fileSize = asmHeader[1];
    newPcb.fileFirstInstruction = asmHeader[2];
    newPcb.prcID = stoi(std::to_string(newPcb.fileLoadAddress) +
                        std::to_string(newPcb.fileEndAddress));
    newPcb.name = filePath;

    printf("prcID = %d\n", newPcb.prcID);
    std::cout << newPcb.name << "\n";

    vMemory.push_back(newPcb);

    return 0;
  }
};
