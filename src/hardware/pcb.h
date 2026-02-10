#ifndef PCB_H
#define PCB_H

#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdio.h>
#include <stdlib.h>

class Pcb {
public:
  // GENERAL
  int prcID;
  int state;
  int arrivalTime;
  int fileLoadAddress;
  int fileFirstInstruction;
  int fileSize;
  int fileEndAddress;
  int kernelMode = 0;

  std::string name;

  int childID = 0;

  // CPU IMAGE FOR CONTEXT SWITCH
  int registers[8];
  int pc;
  int z;
};
#endif
