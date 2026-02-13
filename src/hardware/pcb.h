#ifndef PCB_H
#define PCB_H

#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdio.h>
#include <stdlib.h>

class Pcb {

public:
  int prcID;
  int state;
  int arrivalTime;
  int fileLoadAddress;
  int fileFirstInstruction = 0;
  int fileSize = 0;
  int fileEndAddress = 0;
  int kernelMode = 0;

  std::string name;

  int childID = 0;

  // CPU IMAGE FOR CONTEXT SWITCH
  // CONSTRUCTOR
  int reg[6] = {0};
  int pc = 0;
  int z = 0;

  Pcb()
      : fileLoadAddress{0}, fileEndAddress{0}, fileFirstInstruction{0},
        prcID{0}, pc{0}, name{"default"} {}

  Pcb(std::vector<int> asmHeader, std::string filePath)
      : fileLoadAddress{asmHeader[0]},
        fileEndAddress{asmHeader[0] + (asmHeader[1])}, fileSize{asmHeader[1]},
        fileFirstInstruction{asmHeader[2]}, prcID{asmHeader[0]},
        pc{(fileFirstInstruction / 4) + fileLoadAddress}, name{filePath} {}
  // COPY CONSTRUCTOR
  Pcb(const Pcb &pcb)
      : fileLoadAddress{pcb.fileLoadAddress},
        fileEndAddress{pcb.fileEndAddress}, fileSize{pcb.fileSize},
        fileFirstInstruction{pcb.fileFirstInstruction}, prcID{pcb.prcID},
        pc{pcb.pc}, name{pcb.name} {}
};

#endif
