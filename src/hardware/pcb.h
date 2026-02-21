#ifndef PCB_H
#define PCB_H

#include <bits/stdc++.h>

class Pcb {

public:
  std::string name;
  int pId;
  int pState = 0;
  int pArrivalTime;
  int pLoadAddress;
  int pFirstInstruction = 0;
  int pSize = 0;
  int pEndAddress = 0;
  int pKernelMode = 0;
  int childID = 0;

  // ---------
  // CPU IMAGE
  // ---------

  int reg[6] = {};
  int pc = 0;
  int z = 0;

  // -----------
  // CONSTRUCTOR
  // -----------

  Pcb()
      : pLoadAddress{0}, pEndAddress{0}, pFirstInstruction{0}, pId{0}, pc{0},
        name{"default"} {}

  Pcb(std::vector<int> asmHeader, std::string filePath)
      : pLoadAddress{asmHeader[0]}, pEndAddress{asmHeader[0] + (asmHeader[1])},
        pSize{asmHeader[1]}, pFirstInstruction{asmHeader[2]}, pId{asmHeader[0]},
        pc{pFirstInstruction + pLoadAddress}, name{filePath} {}

  // COPY CONSTRUCTOR
  Pcb(const Pcb &pcb)
      : pLoadAddress{pcb.pLoadAddress}, pEndAddress{pcb.pEndAddress},
        pSize{pcb.pSize}, pFirstInstruction{pcb.pFirstInstruction},
        pId{pcb.pId}, pc{pcb.pc}, name{pcb.name} {}

  std::size_t operator()(const Pcb &pcb) const {
    return std::hash<int>()(pcb.pId);
  }

  void updateState(int newState) {
    printf("[PCB] %d: %d --> ", pId, pState);
    pState = newState;
    printf("%d\n", pState);
  }
};

#endif
