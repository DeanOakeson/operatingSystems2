
#include "scheduler.h"

Scheduler::Scheduler(VirtualMachine &machine) : machine(machine) {}

int Scheduler::firstComeFirstServe() {}

void Scheduler::updateState(Pcb &process, int state) { process.state = state; }

int Scheduler::runProgram() {

  int returnCode = machine.runCpu(machine.ram.vMemory[0]);
  if (returnCode == 100) {
    return 100;
  }
  return 0;
}
