
#include "scheduler.h"

Scheduler::Scheduler(VirtualMachine &machine) : machine(machine) {}

int Scheduler::firstComeFirstServe() {}

// PASS BY REFERENCE
int Scheduler::contextSwitch(Pcb process) {
  machine.PC = process.pc;
  printf("[OS] [SCHEDULER] -- machine.PC = %d\n", machine.PC);
  machine.Z = process.z;
  for (int i = 0; i <= 6; i++) {
    machine.Reg[i] = process.reg[i];
  }

  return 0;
}

void Scheduler::updateState(Pcb &process, int state) { process.state = state; }

int Scheduler::runProgram() {
  // ENTERS PCB INFO INTO CPU
  contextSwitch(machine.ram.vMemory[0]);
  // SPINS UP CPU
  int returnCode = machine.runCpu(machine.ram.vMemory[0]);
  if (returnCode == 100) {
    return 100;
  }
  return 0;
}
