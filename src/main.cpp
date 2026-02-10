#include "./hardware/vm.h"
#include "./kernel/kernel.h"
#include "./shell/shell.h"

int main() {

  VirtualMachine machine;
  Kernel kernel(machine);
  Shell shell(kernel);
  shell.initPath();
  shell.shellLoop();
  return 0;
}
