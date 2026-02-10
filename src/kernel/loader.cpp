#include "loader.h"

Loader::Loader(VirtualMachine &machine) : machine(machine) {}

int Loader::loadProgram(std::string filePath) {

  int fileSize;
  int fileLoadAddress;
  int fileFirstInstruction;

  std::ifstream file(filePath,
                     std::ios::binary); // FILESTREAM ERRORS NEED TO WORK

  if (file.fail()) {
    std::ios_base::iostate state = file.rdstate();

    if (state & std::ios_base::eofbit) {
      return MEM_FSTREAM_ERR_1; // END OF FILE REACHED FAIL
    }
    if (state & std::ios_base::failbit) {
      return MEM_FSTREAM_ERR_2; // NON_FATAL I/O ERROR OCCURED
    }
    if (state & std::ios_base::failbit) {
      return MEM_FSTREAM_ERR_3; // FATAL ERROR OCCURED
    }
  }

  // FIRST READ THE FIRST 3 INTS FOR HEADER VARIABLES //
  file.read(reinterpret_cast<char *>(&fileSize), sizeof(int));
  file.read(reinterpret_cast<char *>(&fileFirstInstruction), sizeof(int));
  file.read(reinterpret_cast<char *>(&fileLoadAddress), sizeof(int));

  fileSize = fileSize / 6; // translated to memory space
  printf("%d\n", fileSize);
  printf("%d\n", fileFirstInstruction);
  printf("%d\n", fileLoadAddress);
  asmHeader = {fileLoadAddress, fileSize, fileFirstInstruction};

  //  MEMORY OVERFLOW
  if (fileLoadAddress + fileSize > MEM_SIZE_KB) {
    printf("[OS][201] --memory overflow, load "
           "cancelled\n");
    return MEM_OVERFLOW;
  }

  // MEMORY IS OCCUPIED
  if (verifyMemoryIsUnoccupied(asmHeader) != true) {
    printf("[OS][202] --attempted to overwrite existing memory, load "
           "cancelled\n");
    return MEM_OVERWRITE;
  }

  // WORD LOADER
  if (fileLoadAddress + fileFirstInstruction != fileLoadAddress) {
    for (int i = fileLoadAddress;
         i < fileFirstInstruction / 4 + fileLoadAddress; i++) {
      for (int j = 0; j < 6; j++) {
        if (j < 4) {
          file.read(reinterpret_cast<char *>(&machine.ram.mem[i][j]),
                    sizeof(unsigned char));
        } else {
          machine.ram.mem[i][j] = 0;
        }
      }
    }
  }

  // LOADER
  for (int i = fileFirstInstruction / 4 + fileLoadAddress;
       i <= fileSize + fileLoadAddress; i++) {
    for (int j = 0; j < 6; j++) {
      file.read(reinterpret_cast<char *>(&machine.ram.mem[i][j]),
                sizeof(unsigned char));
    }
  }

  // PCB creation and memory indicator swap
  updateMemoryIndicators(asmHeader);

  machine.ram.initializePcb(asmHeader, filePath);
  return 0;
}

bool Loader::verifyMemoryIsUnoccupied(std::vector<int> asmHeader) {
  int fileLoadAddress = asmHeader[0];
  int fileSize = asmHeader[1];
  int fileFirstInstruction[2];

  for (int i = fileLoadAddress; i <= fileLoadAddress + fileSize; i++) {
    if (machine.ram.mem[i][6] == 1) {
      return false;
    }
  }
  return true;
}

// FLIPS THE MEMORY OCCUPATION BIT //
void Loader::updateMemoryIndicators(std::vector<int> asmHeader) {
  int fileLoadAddress = asmHeader[0];
  int fileSize = asmHeader[1];
  int fileFirstInstruction[2];

  for (int i = fileLoadAddress; i <= fileLoadAddress + fileSize; i++) {
    if (machine.ram.mem[i][6] == 0)
      machine.ram.mem[i][6] = 1;
    else if (machine.ram.mem[i][6] == 1)
      machine.ram.mem[i][6] = 0;
  }
}
