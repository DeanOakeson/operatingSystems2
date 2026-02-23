#include "scheduler.h"
// -----------
// Constructor
// -----------

Scheduler::Scheduler(VirtualMachine &machine) : machine(machine) {}

// ----------------
// Scheduling algos
// ----------------

int Scheduler::firstComeFirstServe() {
  int returnCode = 0;

  // IF ALL QUEUES ARE EMPTY IDLE THE CLOCK
  if (newQueue.empty() && readyQueue.empty() && waitingQueue.empty() &&
      runningQueue.empty()) {
    machine.idle();
  }

  // SEND ALL NEW PCBS TO READY
  if (!newQueue.empty()) {
    pPcb = static_cast<Pcb *>(newQueue.front());
    pPcb->updateState(READY);
    newQueue.pop();
    readyQueue.push_back(pPcb);
  }

  // IF RUNNING QUEUE IS EMPTY PROCESS PREP
  if (runningQueue.empty() && !readyQueue.empty()) {
    pPcb = static_cast<Pcb *>(readyQueue.front());
    pPcb->updateState(RUNNING);
    clearCpu();
    contextToCpu(*pPcb);
    runningQueue.push(pPcb);
    readyQueue.pop_front();
  }

  // PROCESS RUN
  if (pPcb != NULL) {
    returnCode = machine.fetchDecodeExecute(*pPcb);
  }

  // SWI RETURN
  if (returnCode >= 20) {
    pPcb->updateState(WAITING);
    runningQueue.pop();
    waitingQueue.push(pPcb);
    contextToPcb(*pPcb);
    clearCpu();
    interruptServiceRoutine(*pPcb, returnCode);
    // printf("[OS][SCHEDULER] -- interrupt handeled\n");
    waitingQueue.pop();
    readyQueue.push_back(pPcb);
    pPcb->updateState(READY);
    return returnCode;
  }

  // PROCESS TERMINATION
  if (returnCode == 10) {
    pPcb->updateState(TERMINATED);
    runningQueue.pop();
    terminatedQueue.push(pPcb);
    printf("[OS][SCHEDULER] prc %s is terminating\n", pPcb->name.c_str());
    deallocateMemory(*pPcb);
    terminatedQueue.pop();
    pPcb = NULL;
    return returnCode;
  }

  // printf("[OS][SCHEDULER] clock = %d\n", machine.clock);
  return 0;
}

// ------------------
// Single Program Run
// ------------------

int Scheduler::singleProgram(Pcb *pcb) {
  pPcb = pcb;

  // READY CPU and add CONTEXT
  if (pPcb->pState == NEW) {
    pPcb->updateState(READY);
    clearCpu();
    newQueue.pop();
    readyQueue.push_front(pPcb);
    pPcb->updateState(RUNNING);
    readyQueue.pop_front();
    runningQueue.push(pPcb);
    contextToCpu(*pPcb);
  }

  if (pPcb->pState == READY) {
    pPcb->updateState(RUNNING);
    readyQueue.pop_front();
    runningQueue.push(pPcb);
    contextToCpu(*pPcb);
  }

  int returnCode = machine.fetchDecodeExecute(*pPcb);

  // RETURN CODE = SWI
  if (returnCode >= 20) {
    pPcb->updateState(WAITING);
    runningQueue.pop();
    waitingQueue.push(pPcb);
    contextToPcb(*pPcb);
    clearCpu();
    interruptServiceRoutine(*pPcb, returnCode);
    printf("[OS][SCHEDULER] -- interrupt handeled\n");
    waitingQueue.pop();
    readyQueue.push_front(pPcb);
    pPcb->updateState(READY);
    return returnCode;
  }

  if (returnCode == 10) {
    pPcb->updateState(TERMINATED);
    runningQueue.pop();
    terminatedQueue.push(pPcb);
    printf("[OS][SCHEDULER] prc %s is terminating\n", pPcb->name.c_str());
    deallocateMemory(*pPcb);
    terminatedQueue.pop();
    pPcb = NULL;
    return returnCode;
  }
  return returnCode;
}

// --------------
// PCB and QUEUE Managment
// -------------

int Scheduler::allocateMemory(std::vector<int> asmHeader,
                              std::string filePath) {

  // PCB CREATION //
  Pcb *pPcb = new Pcb(asmHeader, filePath);
  machine.ram.vMemory.push_back(pPcb);
  machine.ram.vMemoryLookup[pPcb->name] = machine.ram.vMemory.size() - 1;

  // MEMORY ALLOCATION
  for (int i = pPcb->pLoadAddress; i <= pPcb->pLoadAddress + pPcb->pSize; i++) {
    machine.ram.mem[i][1] = 1;
  }

  // PUSH PCB TO NEW //
  pPcb->updateState(NEW);
  printf("[OS][SCHEDULER][ALLOCATE] Id = %d\n", pPcb->pId);

  printf("[OS][SCHEDULER][ALLOCATE] state = %d\n", pPcb->pState);
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

  return machine.ram.vMemory[index];
}

bool Scheduler::empty() {
  if (readyQueue.empty() && newQueue.empty() && runningQueue.empty() &&
      waitingQueue.empty()) {
    return true;
  }
  return false;
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

int Scheduler::setClock(Pcb &process) {
  process.pArrivalTime = machine.clock;
  return 0;
}

void Scheduler::deallocateMemory(Pcb &process) {
  // FLIPS MEM OCCUPATION BIT AT EACH ADDRESS BACK TO ZERO
  printf("[OS][SCHEDULER][DEALLOCATE][%s]\n\n", process.name.c_str());

  // WORKS
  for (int i = process.pLoadAddress; i <= process.pLoadAddress + process.pSize;
       i++) {
    machine.ram.mem[i][1] = 0;
  }

  // IFY
  machine.ram.vMemory.erase(machine.ram.vMemory.begin() +
                            machine.ram.vMemoryLookup.at(process.name));
  machine.ram.vMemoryLookup.erase(process.name);
}

// -------------------------
// Interrupt Service Routine
// -------------------------

void Scheduler::interruptServiceRoutine(Pcb &process, int returnCode) {
  printf("[OS][SCHEDULER][IRS]\n");
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
