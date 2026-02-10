#include "kernel.h"

Kernel::Kernel(VirtualMachine &machine)
    : machine(machine), errorHandler(machine), loader(machine),
      scheduler(machine) {}
