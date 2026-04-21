#include "loader.h"

//------------
// CONSTRUCTOR
//------------

Loader::Loader(VirtualMachine &machine) : machine(machine) {};

void Loader::setVerbosityFlag() {
  if (verbosityFlag == true) {
    verbosityFlag = false;
    return;
  }
  verbosityFlag = true;
  return;
}

//------
// LOAD
//------

// loadProgram(filePath)
// INPUT - takes a file path in a string format
//
// OUTPUT SUCCESS - returns a vector that contains 0 and a vector
// containing the header information that was loaded from the asm file.
//
// OUTPUT FAIL - returns a return code that indicates the problem that occured
// and an empty vector.

std::tuple<int, std::vector<int>> Loader::loadProgram(std::string filePath) {

  int fileSize;
  int fileLoadAddress;
  int fileFirstInstruction;

  std::ifstream file(filePath,
                     std::ios::binary); // FILESTREAM ERRORS NEED TO WORK

  if (!file.is_open()) {
    if (verbosityFlag == true) {
      printf("[LD]::ldpr - ERROR/200 - file not found\n");
    }
    return std::make_tuple(200, asmHeader);
    ;
  }
  // FIRST READ THE FIRST 3 INTS FOR HEADER VARIABLES //
  file.read(reinterpret_cast<char *>(&fileSize), sizeof(int));
  file.read(reinterpret_cast<char *>(&fileFirstInstruction), sizeof(int));
  file.read(reinterpret_cast<char *>(&fileLoadAddress), sizeof(int));

  asmHeader = {fileLoadAddress, fileSize, fileFirstInstruction};

  if (verbosityFlag == true) {
    printf("[LD]::ldpr - fileSize = %d\n", asmHeader[1]);
    printf("[LD]::ldpr - fileFirstInstruction = %d\n", asmHeader[2]);
    printf("[LD]::ldpr - fileLoadAddress = %d\n", fileLoadAddress);
  }
  //  MEMORY OVERFLOW
  if (fileLoadAddress + fileSize > MEM_SIZE_KB) {
    if (verbosityFlag == true) {
      printf("[LD]::ldpr - 201 - memory overflow, load "
             "cancelled\n");
    }
    return std::make_tuple(201, asmHeader);
  }

  // MEMORY IS OCCUPIED
  if (verifyMemoryIsUnoccupied(asmHeader) != true) {
    if (verbosityFlag == true) {
      printf("[LD]::ldpr - ERROR/202 - attempted to overwrite existing memory, "
             "load "
             "cancelled\n");
    }
    return std::make_tuple(202, asmHeader);
    ;
  }

  // LOADER
  for (int i = fileLoadAddress; i <= fileSize + fileLoadAddress; i++) {
    file.read(reinterpret_cast<char *>(&machine.ram.mem[i][0]),
              sizeof(unsigned char));
  }

  return std::make_tuple(0, asmHeader);
  ;
}

//------------------
// PRIVATE FUNCTIONS
//------------------

bool Loader::verifyMemoryIsUnoccupied(std::vector<int> asmHeader) {
  int fileLoadAddress = asmHeader[0];
  int fileSize = asmHeader[1];
  int fileFirstInstruction = asmHeader[2];

  for (int i = fileLoadAddress; i <= fileLoadAddress + fileSize; i++) {
    if (machine.ram.mem[i][1] == 1) {
      return false;
    }
  }
  return true;
}

int Loader::allocateMemory(Pcb &process) {

  // memory allocation //
  for (int i = process.pLoadAddress; i <= process.pLoadAddress + process.pSize;
       i++) {
    machine.ram.mem[i][1] = 1;
  }
  if (verbosityFlag == true) {
    printf("[LD]::allo - pId = %d\n", process.pId);
    printf("[LD]::allo - pState = %d\n", process.pState);
  }

  return 0;
}

int Loader::allocateSharedMemory(int size) {
  // finds and allocates a space in memory large enough for shared mempory
  size = size * 6; // translate to memspace
  int location = 0;
  int count = 0; // iterate through mem

  for (int i = 0; i <= machine.ram.capacity; i++) {
    // std::cout << i << "[" << static_cast<int>(machine.ram.mem[i][1]) << "]"
    //           << std::endl;
    // location is initizialed if count == 0
    if (count == 0) {
      location = i;
    }
    // IF a count is successfully reached then allocate
    if (count >= size) {
      break;
    }

    // if the memory marker bit is 0 then count up
    if (static_cast<int>(machine.ram.mem[i][1]) == 0) {
      count += 1;
      continue;
    }
    // if memory marker bit is 1 then it is occupied
    if (static_cast<int>(machine.ram.mem[i][1]) == 1) {
      count = 0;
      continue;
    }

    // if i == capacity
    if (verbosityFlag == true) {
      std::cout
          << "[LD]::ashm - could not find a space to allocate shared memory"
          << std::endl;
    }
    return -1;
  }
  // allocate found memory
  for (int j = location; j <= location + size; j++) {
    machine.ram.mem[j][1] = 1;
  }

  return location;
}
Pcb *Loader::createPcb(std::vector<int> asmHeader, std::string filePath) {
  Pcb *pPcb = new Pcb(asmHeader, filePath);
  pPcb->pId = currentIdCount;

  // PUSH ONTO vMEMORY and vMEMORY LOOKUP
  machine.ram.vMemory.push_back(pPcb);
  machine.ram.vMemoryLookup[pPcb->name] = machine.ram.vMemory.size() - 1;
  currentIdCount += 1;
  if (verbosityFlag == true) {
    std::cout << "[LD]::cpcb - pcb" << pPcb->pId << " created" << std::endl;
  }
  return pPcb;
}
