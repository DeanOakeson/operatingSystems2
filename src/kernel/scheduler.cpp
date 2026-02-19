// the scheduler needs an observer pattern to watch for new process loaded in
// vMem
#include "scheduler.h"

Scheduler::Scheduler(VirtualMachine &machine) : machine(machine) {}

int Scheduler::firstComeFirstServe() {}

// PASS BY REFERENCE
int Scheduler::contextSwitch(Pcb &process) {
  machine.PC = process.pc;
  printf("[OS] [SCHEDULER] -- machine.PC = %d\n", machine.PC);
  machine.Z = process.z;
  for (int i = 0; i <= 6; i++) {
    machine.Reg[i] = process.reg[i];
  }

  return 0;
}

void Scheduler::updateState(Pcb &process, int state) { process.state = state; }

int Scheduler::runProgram(Pcb &process) {

  contextSwitch(process);

  int returnCode = machine.runCpu(process);

  if (returnCode == 100) {
    return 100;
  }

  return 0;
}

int Scheduler::initializePcb(std::vector<int> asmHeader, std::string filePath) {

  // create pcb from asmHeader//
  Pcb newPcb(asmHeader, filePath);
  std::cout << "[VM] [RAM]" << newPcb.name << " = ";
  machine.ram.vMemory.push_back(newPcb);
  machine.ram.vMemoryLookup[newPcb.name] = machine.ram.vMemory.size() - 1;
  printf("%d\n", machine.ram.vMemoryLookup[newPcb.name]);

  // push pcb to scheduling queues//
  newQueue.push(newPcb);

  return 0;
}

Pcb Scheduler::getPcb(std::string filePath) {
  Pcb pcb = machine.ram.vMemory[machine.ram.vMemoryLookup[filePath]];
  return pcb;
}
