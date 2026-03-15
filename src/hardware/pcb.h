#ifndef PCB_H
#define PCB_H

#include <bits/stdc++.h>

class Pcb {

public:
  std::string name;

  int pId = -1;
  int pState = 0;
  int pArrivalTime;
  int pLoadAddress;
  int pFirstInstruction = 0;
  int pSize = 0;
  int pEndAddress = 0;
  int pKernelMode = 0;
  Pcb *pChild = NULL;

  std::vector<int> cpuTimeSlices;

  // ---------
  // CPU IMAGE
  // ---------

  int Reg[6] = {};
  int PC = 0;
  int Z = 0;

  // -----------
  // CONSTRUCTOR
  // -----------

  // DEFAULT
  Pcb()
      : pLoadAddress{0}, pEndAddress{0}, pFirstInstruction{0}, pId{0}, PC{0},
        name{"default"} {}

  // CONSTRUCTOR
  Pcb(std::vector<int> asmHeader, std::string filePath)
      : pLoadAddress{asmHeader[0]}, pEndAddress{asmHeader[0] + (asmHeader[1])},
        pSize{asmHeader[1]}, pFirstInstruction{asmHeader[2]}, pId{0},
        pArrivalTime{asmHeader[3]}, PC{pFirstInstruction + pLoadAddress},
        name{filePath} {}

  // COPY CONSTRUCTOR FOR FORK
  Pcb(const Pcb &pcb)
      : pLoadAddress{pcb.pLoadAddress}, pEndAddress{pcb.pEndAddress},
        pSize{pcb.pSize}, pFirstInstruction{pcb.pFirstInstruction},
        pId{pcb.pId + 1}, PC{pcb.PC}, name{pcb.name + ".child"} {}

  std::size_t operator()(const Pcb &pcb) const {
    return std::hash<int>()(pcb.pId);
  }

  void updateState(int newState) {
    // printf("[PCB][PRC %d]: %d --> ", pId, pState);
    pState = newState;
    // printf("%d\n", pState);
  }
  void captureTimeSlice(int clockImage) { cpuTimeSlices.push_back(clockImage); }
};

#endif
