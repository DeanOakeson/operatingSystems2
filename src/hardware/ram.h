#include "pcb.h"
#include <string.h>
#include <unordered_map>

#define MEM_SIZE_KB 12 * 1024 // 12 KB

class Ram {
public:
  unsigned char mem[MEM_SIZE_KB * 8][2];
  // VMEM LOOKUP TABLE// name : index in vMemory
  std::unordered_map<std::string, int> vMemoryLookup;
  std::vector<Pcb> vMemory;
};
