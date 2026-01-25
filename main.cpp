#include "include/kernel.h"
#include "include/shell.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdlib.h>

int main() {

  Kernel Kernel;
  Shell Shell;
  Ram MainMemory;
  VirtualMachine Cpu;
  Kernel.initHardware(Cpu, MainMemory);
  Shell.initPath();
  Shell.shellLoop();
  return 0;
}
