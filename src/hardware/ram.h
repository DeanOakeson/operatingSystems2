#include "pcb.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <unordered_map>

#define MEM_SIZE_KB 12 * 1024 // 512 KB

class Ram {
public:
  // OBJECTS//
  unsigned char mem[MEM_SIZE_KB * 8][2];

  // CREATE VMEM //
  std::vector<Pcb> vMemory;
  // std::unordered_map<Pcb, std::string> hashVMemory;

  int initializePcb(std::vector<int> asmHeader, std::string filePath) {
    Pcb newPcb(asmHeader, filePath);
    std::cout << "[VM] [RAM]" << newPcb.name << "\n";
    vMemory.push_back(newPcb);
    return 0;
  }
};
