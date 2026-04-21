#include "pcb.h"
#include "shm.h"

#define MEM_SIZE_KB 12 * 1024 // 2 KB -- 12/6 = 2KB

class Ram {
public:
  int capacity = MEM_SIZE_KB;
  std::array<unsigned char, 2> mem[MEM_SIZE_KB] = {};
  // VMEM LOOKUP TABLE// name : index in vMemory
  std::unordered_map<std::string, int> vMemoryLookup;
  std::vector<Pcb *> vMemory;

  Pcb *getPcb(std::string name) {
    try {
      int index = vMemoryLookup.at(name);
      return vMemory[index];

    } catch (const std::out_of_range &e) {
      return NULL;
    }
  }
};
