#include "include/ram.h"
#include "include/shell.h"
#include "include/vm.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  VirtualMachine Machine;
  Ram MainMemory;

  Shell Shell;

  // printf("\n// MEMORY LAYOUT //\n");
  // printf("%d ", MainMemory.mem[12][0]);
  // printf("%d ", MainMemory.mem[12][1]);
  // printf("%d ", MainMemory.mem[12][2]);
  // printf("%d ", MainMemory.mem[12][3]);
  // printf("%d ", MainMemory.mem[12][4]);
  // printf("%d\n", MainMemory.mem[12][5]);
  //
  // printf("%d ", MainMemory.mem[13][0]);
  // printf("%d ", MainMemory.mem[13][1]);
  // printf("%d ", MainMemory.mem[13][2]);
  // printf("%d ", MainMemory.mem[13][3]);
  // printf("%d ", MainMemory.mem[13][4]);
  // printf("%d\n", MainMemory.mem[13][5]);
  //
  // printf("%d ", MainMemory.mem[14][0]);
  // printf("%d ", MainMemory.mem[14][1]);
  // printf("%d ", MainMemory.mem[14][2]);
  // printf("%d ", MainMemory.mem[14][3]);
  // printf("%d ", MainMemory.mem[14][4]);
  // printf("%d\n", MainMemory.mem[14][5]);
  //
  // printf("%d ", MainMemory.mem[15][0]);
  // printf("%d ", MainMemory.mem[15][1]);
  // printf("%d ", MainMemory.mem[15][2]);
  // printf("%d ", MainMemory.mem[15][3]);
  // printf("%d ", MainMemory.mem[15][4]);
  // printf("%d\n", MainMemory.mem[15][5]);
  //
  // printf("%d ", MainMemory.mem[16][0]);
  // printf("%d ", MainMemory.mem[16][1]);
  // printf("%d ", MainMemory.mem[16][2]);
  // printf("%d ", MainMemory.mem[16][3]);
  // printf("%d ", MainMemory.mem[16][4]);
  // printf("%d\n", MainMemory.mem[16][5]);
  //
  // printf("%d ", MainMemory.mem[17][0]);
  // printf("%d ", MainMemory.mem[17][1]);
  // printf("%d ", MainMemory.mem[17][2]);
  // printf("%d ", MainMemory.mem[17][3]);
  // printf("%d ", MainMemory.mem[17][4]);
  // printf("%d\n", MainMemory.mem[17][5]);
  // printf("\n// REGISTERS //");
  //

  Shell.initShell(Machine, MainMemory);
  Shell.shellLoop();
  return 0;
}
