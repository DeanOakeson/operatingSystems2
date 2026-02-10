#ifndef VM_H
#define VM_H

#include "ram.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdio.h>
#include <stdlib.h>

class VirtualMachine {
public:
  int reg[6];
  int PC = 0;
  int SP = 0;
  int FP = 0;
  int SL = 0;
  int Z = 0;
  int SB = 0;
  int byteOffset = 0;
  int wordOffset = 0;
  int swiOpCode = 0;
  int clock = 0;
  Ram ram;

  int runCpu(Pcb &process);
};

#endif
