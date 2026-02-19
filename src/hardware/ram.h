#include "pcb.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <unordered_map>

#define MEM_SIZE_KB 12 * 1024 // 12 KB

class Ram {
public:
  // MEMORY //
  unsigned char mem[MEM_SIZE_KB * 8][2];
  // VMEM LOOKUP TABLE// name : index in vMemory
  std::unordered_map<std::string, int> vMemoryLookup;
  // VMEM//
  std::vector<Pcb> vMemory;

  int initializePcb(std::vector<int> asmHeader, std::string filePath) {

    Pcb newPcb(asmHeader, filePath);
    std::cout << "[VM] [RAM]" << newPcb.name << " = ";
    vMemory.push_back(newPcb);
    vMemoryLookup[newPcb.name] = vMemory.size() - 1;
    printf("%d\n", vMemoryLookup[newPcb.name]);

    return 0;
  }
};
