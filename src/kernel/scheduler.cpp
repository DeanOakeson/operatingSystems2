#include "scheduler.h"

// -----------
// CONSTRUCTOR
// -----------

Scheduler::Scheduler(VirtualMachine &machine) : machine(machine) {}

// ----------------
// SCHEDULING ALGOS
// ----------------

int Scheduler::firstComeFirstServe() {
  Pcb *pPcb;
  int returnCode;
  int passNumber = 0;
  while (!newQueue.empty() | !readyQueue.empty() | !waitingQueue.empty()) {
    passNumber += 1;

    // SEND ALL PCBS IN NEW TO READY
    while (!newQueue.empty()) {
      pPcb = static_cast<Pcb *>(newQueue.front());
      pPcb->updateState(READY);
      newQueue.pop();
      readyQueue.push(pPcb);
    }

    // WHILE READY QUEUE IS NOT EMPTY
    while (!readyQueue.empty()) {
      pPcb = static_cast<Pcb *>(readyQueue.front());
      pPcb->updateState(RUNNING);
      runningQueue.push(pPcb);
      readyQueue.pop();

      printf("[OS][SCHEDULER] -- cpu running prc %d\n", pPcb->pId);
      returnCode = runProgram(*pPcb);
      printf("[OS][SCHEDULER] -- returnCode %d\n", returnCode);

      // VM RETURNS > 20 PCB IS SENT TO WAITING
      if (returnCode >= 20) {
        pPcb->updateState(WAITING);
        runningQueue.pop();
        waitingQueue.push(pPcb);
        contextToPcb(*pPcb);
        clearCpu();
        interruptServiceRoutine(*pPcb, returnCode);
        printf("[OS][SCHEDULER] -- interrupt handeled\n");
        printf("pPCB pc = %d\n", pPcb->pc);
        pPcb->pc += 6;
        waitingQueue.pop();
        readyQueue.push(pPcb);
        pPcb->updateState(READY);
        break;
      }

      // VM RETURNS A 0 PCB IS SENT TO TERMINATION
      if (returnCode == 0) {
        pPcb->updateState(TERMINATED);
        runningQueue.pop();
        terminatedQueue.push(pPcb);
        break;
      }
    }
    printf("[OS][SCHEDULER] FCFS pass = %d\n", passNumber);
  }
  return 0;
}

// ------------------
// SINGLE PROGRAM RUN
// ------------------

int Scheduler::runProgram(Pcb &process) {

  contextToCpu(process);

  int returnCode = machine.runCpu(process);

  return returnCode;
}

// --------------
// PCB MANAGMENT
// -------------

int Scheduler::createPcb(std::vector<int> asmHeader, std::string filePath) {

  // PCB CREATION //
  Pcb *pPcb = new Pcb(asmHeader, filePath);
  machine.ram.vMemory.push_back(*pPcb);
  machine.ram.vMemoryLookup[pPcb->name] = machine.ram.vMemory.size() - 1;

  // PUSH PCB TO NEW //
  pPcb->updateState(NEW);
  newQueue.push(pPcb);

  return 0;
}

Pcb *Scheduler::getPcb(std::string filePath) {
  int index;
  try {
    index = machine.ram.vMemoryLookup.at(filePath);
  } catch (std::out_of_range) {
    printf(
        "[OS][ERROR][302] --attempted vMemoryLookup.at() with false program\n");
    return NULL;
  }

  return &machine.ram.vMemory[index];
}

// ----------------
// CONTEXT FUNCTIONS
// ----------------

int Scheduler::contextToCpu(Pcb &process) {
  // LOAD CONTENTS FROM CPU TO PCB
  // LOAD PCB CONTENTS INTO CPU
  machine.PC = process.pc;
  machine.Z = process.z;
  printf("[OS][SCHEDULER][CONTEXTTOCPU] -- %d\n", machine.PC);

  for (int i = 0; i <= 6; i++) {
    machine.Reg[i] = process.reg[i];
  }

  return 0;
}

int Scheduler::contextToPcb(Pcb &process) {
  // LOAD CONTENTS FROM CPU TO PCB
  // LOAD PCB CONTENTS INTO CPU
  process.pc = machine.PC;
  printf("[OS][SCHEDULER][CONTEXTTOPCB] -- %d\n", process.pc);
  process.z = machine.Z;
  for (int i = 0; i <= 6; i++) {
    process.reg[i] = machine.Reg[i];
  }

  return 0;
}

int Scheduler::clearCpu() {
  machine.PC = 0;
  machine.Z = 0;
  for (int i = 0; i <= 6; i++) {
    machine.Reg[i] = 0;
  }
  return 0;
}

// -------------------------
// Interrupt Service Routine
// -------------------------

void Scheduler::interruptServiceRoutine(Pcb &process, int returnCode) {
  printf("[OS][SCHEDULER] -- irs started\n");
  switch (returnCode) {
  case PRINT:
    printf("print\n");
    break;
  case HALT:
    printf("halt\n");
    break;
  case FORK:
    printf("halt\n");
    break;
  case WAIT:
    printf("wait\n");
    break;
  case INPUT:
    printf("input\n");
    break;
  }
  return;
}
