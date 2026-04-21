#ifndef PCB_H
#define PCB_H

#include "shm.h"
#include <bits/stdc++.h>

class Pcb {

public:
  std::string name;
  std::vector<int> cpuTimeSlices = {};

  int pPriority = 0;
  int pQuantumCount = 0;
  int pId = -1;
  int semId = -1;
  int pState = 0;
  int pLoadAddress = 0;
  int pEndAddress = 0;
  int pFirstInstruction = 0;
  int pSize = 0;
  int pKernelMode = 0;
  int pArrivalTime = 0;
  int pTerminationTime = 0;

  int wait = 0;
  int response = 0;
  int turnAround = 0;

  // shared mempointer
  Shm *sharedMemory = NULL;
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
      : name{"default"}, pId{0}, pLoadAddress{0}, pEndAddress{0},
        pFirstInstruction{0}, PC{0} {}

  // CONSTRUCTOR
  Pcb(std::vector<int> asmHeader, std::string filePath)
      : name{filePath}, pId{0}, pLoadAddress{asmHeader[0]},
        pEndAddress{asmHeader[0] + (asmHeader[1])},
        pFirstInstruction{asmHeader[2]}, pSize{asmHeader[1]},
        pArrivalTime{asmHeader[3]}, PC{pFirstInstruction + pLoadAddress} {}

  // COPY CONSTRUCTOR FOR FORK
  Pcb(const Pcb &pcb)
      : name{pcb.name + ".child"}, pId{pcb.pId + 1},
        pLoadAddress{pcb.pLoadAddress}, pEndAddress{pcb.pEndAddress},
        pFirstInstruction{pcb.pFirstInstruction}, pSize{pcb.pSize}, wait{0},
        response{0}, turnAround{0}, PC{pcb.PC} {}

  std::size_t operator()(const Pcb &pcb) const {
    return std::hash<int>()(pcb.pId);
  }

  void updateState(int newState) { pState = newState; }
  void captureTimeSlice(int clockImage) {
    calculateWait(clockImage);
    cpuTimeSlices.push_back(clockImage);
  }

  void promotePriority() {
    if (pPriority > 0)
      pPriority--;
    pQuantumCount = 0;
    return;
  }
  void demotePriority() {
    if (pPriority < 2)
      pPriority++;
    pQuantumCount = 0;
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

  Shm *openSharedMemory(int memId, int size, int address) {
    Shm *newSharedMemory = new Shm(memId, size, address, name);
    sharedMemory = newSharedMemory;
    sharedMemory->prcOne = name;
    sharedMemory->sLinkedPrcCount++;
    return newSharedMemory;
  }

  void linkSharedMemory(Shm *newSharedMemory) {
    sharedMemory = newSharedMemory;
    sharedMemory->prcTwo = name;
    sharedMemory->sLinkedPrcCount++;
  }
};

#endif
