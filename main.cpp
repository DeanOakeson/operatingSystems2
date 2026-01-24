#include "include/shell.h"
#include "include/vm.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdlib.h>

int main() {

  VirtualMachine Machine;
  Shell Shell;
  Shell.initPath(Machine);
  Shell.shellLoop();
  return 0;
}
