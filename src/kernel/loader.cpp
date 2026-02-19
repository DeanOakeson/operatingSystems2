#include "loader.h"

Loader::Loader(VirtualMachine &machine) : machine(machine) {};

std::tuple<int, std::vector<int>> Loader::loadProgram(std::string filePath) {
  std::cout << filePath << "\n";

  int fileSize;
  int fileLoadAddress;
  int fileFirstInstruction;

  std::ifstream file(filePath,
                     std::ios::binary); // FILESTREAM ERRORS NEED TO WORK

  if (!file.is_open()) {
    printf("[OS][LOADER][200] -- file not found\n");
    return std::make_tuple(200, asmHeader);
    ;
  }
  // FIRST READ THE FIRST 3 INTS FOR HEADER VARIABLES //
  file.read(reinterpret_cast<char *>(&fileSize), sizeof(int));
  file.read(reinterpret_cast<char *>(&fileFirstInstruction), sizeof(int));
  file.read(reinterpret_cast<char *>(&fileLoadAddress), sizeof(int));

  asmHeader = {fileLoadAddress, fileSize, fileFirstInstruction};
  printf("[OS][LOADER] -- fileSize = %d\n", asmHeader[1]);
  printf("[OS][LOADER] -- fileFirstInstruction = %d\n", asmHeader[2]);
  printf("[OS][LOADER] -- fileLoadAddress = %d\n", fileLoadAddress);

  //  MEMORY OVERFLOW
  if (fileLoadAddress + fileSize > MEM_SIZE_KB) {
    printf("[OS][201] --memory overflow, load "
           "cancelled\n");
    return std::make_tuple(201, asmHeader);
  }

  // MEMORY IS OCCUPIED
  if (verifyMemoryIsUnoccupied(asmHeader) != true) {
    printf("[OS][202] --attempted to overwrite existing memory, load "
           "cancelled\n");
    return std::make_tuple(202, asmHeader);
    ;
  }

  // LOADER
  for (int i = fileLoadAddress; i <= fileSize + fileLoadAddress; i++) {
    file.read(reinterpret_cast<char *>(&machine.ram.mem[i][0]),
              sizeof(unsigned char));
  }

  // PCB creation and memory indicator swap
  updateMemoryIndicators(asmHeader);
  return std::make_tuple(0, asmHeader);
  ;
}

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

// FLIPS THE MEMORY OCCUPATION BIT //
void Loader::updateMemoryIndicators(std::vector<int> asmHeader) {
  int fileLoadAddress = asmHeader[0];
  int fileSize = asmHeader[1];
  int fileFirstInstruction = asmHeader[2];

  for (int i = fileLoadAddress; i <= fileLoadAddress + fileSize; i++) {
    if (machine.ram.mem[i][1] == 0)
      machine.ram.mem[i][1] = 1;
    else if (machine.ram.mem[i][1] == 1)
      machine.ram.mem[i][1] = 0;
  }
}
