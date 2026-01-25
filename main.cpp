#include "include/kernel.h"
#include "include/shell.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdlib.h>

int main() {

  Kernel Kernel;
  Shell Shell;
  VirtualMachine Cpu;
  Kernel.initHardware(Cpu);
  Shell.initPath();
  Shell.shellLoop();
  return 0;
}
