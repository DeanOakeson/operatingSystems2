#ifndef PCB_H
#define PCB_H

#include <bits/stdc++.h>

class Pcb {

public:
  std::string name;
  std::vector<int> cpuTimeSlices = {};

  int pPriority = 0;
  int pQuantumCount = 0;

  int pId = -1;
  int pState = 0;
  int pLoadAddress;
  int pEndAddress = 0;
  int pFirstInstruction = 0;
  int pSize = 0;
  int pKernelMode = 0;
  int pArrivalTime = 0;
  int pTerminationTime = 0;

  int wait = 0;
  int response = 0;
  int turnAround = 0;

  Pcb *pChild = NULL;

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
        pId{pcb.pId + 1}, PC{pcb.PC}, name{pcb.name + ".child"}, wait{0},
        response{0}, turnAround{0} {}

  std::size_t operator()(const Pcb &pcb) const {
    return std::hash<int>()(pcb.pId);
  }

  void updateState(int newState) {
    //   printf("[PCB][PRC %d]: %d --> ", pId, pState);
    pState = newState;
    //  printf("%d\n", pState);
  }
  void captureTimeSlice(int clockImage) {
    calculateWait(clockImage);
    cpuTimeSlices.push_back(clockImage);
  }

  void promotePriority() {
    if (pPriority < 2)
      pPriority++;
    std::cout << name << " got a promotion to priority " << pPriority
              << std::endl;
    return;
  }
  void demotePriority() {
    if (pPriority > 0)
      pPriority--;
    std::cout << name << " got a demotion to priority " << pPriority
              << std::endl;
    return;
  }
  void incrementQuatumCount() { pQuantumCount++; }
  void decrementQuantumCount() { pQuantumCount--; }
  void resetQuantumCount() { pQuantumCount = 0; }

  void calculateWait(int clockImage) {
    int lastTimeSlice;
    if (cpuTimeSlices.empty()) {
      lastTimeSlice = pArrivalTime;
    } else {
      lastTimeSlice = cpuTimeSlices.back();
    }

    int gap =
        clockImage - lastTimeSlice - 1; // subtract 1, consecutive = 0 wait
    if (gap > 0) {
      wait += gap;
    }
  }

  void calculateResponse() { response = cpuTimeSlices[0] - pArrivalTime; }
  void calculateTurnAround() { turnAround = pTerminationTime - pArrivalTime; }
};

#endif
