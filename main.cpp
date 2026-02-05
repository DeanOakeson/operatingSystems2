#include "include/kernel.h"
#include "include/shell.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdlib.h>

// FIVE STATES AND a little CPU SCHEDULING, we need arriving time in PCB//
// PCB //
// FORK and EXCECUTE //
// RUN OSX COMPILER IN SHELL (create a shell command) //

int main() {

  Kernel Kernel;
  Shell Shell;
  VirtualMachine Cpu;
  Kernel.initHardware(Cpu);
  Shell.initPath();
  Shell.shellLoop();
  return 0;
}
