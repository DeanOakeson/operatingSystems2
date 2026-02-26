#include "scheduler.h"
// -----------
// Constructor
// -----------

Scheduler::Scheduler(VirtualMachine &machine) : machine(machine) {}

// ----------------
// Scheduling algos
// ----------------

int Scheduler::firstComeFirstServe() {
  // FIRST COME FIRST SERVE //
  // must be wrapped in a loop to run an entire program //
  // each function call advances the clock one step //

  int returnCode = 0;

  // SEND A NEW PCB TO READY
  while (!newQueue.empty()) {
    pPcb = static_cast<Pcb *>(newQueue.front());
    pPcb->updateState(READY);
    newQueue.pop();
    readyQueue.push_back(pPcb);
  }

  // if running queue is empty process prep
  if (runningQueue.empty() && !readyQueue.empty()) {
    pPcb = static_cast<Pcb *>(readyQueue.front());
    pPcb->updateState(RUNNING);
    clearCpu();
    contextToCpu(*pPcb);
    runningQueue.push(pPcb);
    readyQueue.pop_front();
  }

  // run process in running
  if (pPcb != NULL) {
    pPcb = static_cast<Pcb *>(runningQueue.front());
    returnCode = machine.fetchDecodeExecute(*pPcb);
  } else {
    machine.idle();
  }

  // swi return
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

  // process termination
  if (returnCode == 10) {
    pPcb->updateState(TERMINATED);
    runningQueue.pop();
    terminatedQueue.push(pPcb);
    printf("[SCH][FCFS] -- Prc %s is terminating\n", pPcb->name.c_str());
    deallocateMemory(*pPcb);
    terminatedQueue.pop();
    pPcb = NULL;
    return returnCode;
  }

  // DEBUG//
  //  printf("[OS][SCHEDULER] clock = %d\n", machine.clock);
  return 0;
}

// --------------
// PCB and QUEUE Managment
// -------------

Pcb *Scheduler::allocateMemory(std::vector<int> asmHeader,
                               std::string filePath) {

  // creates pcb and flips the memory allocation bits to 1

  // pcb creation //
  Pcb *pPcb = new Pcb(asmHeader, filePath);
  pPcb->pId = currentIdCount;
  machine.ram.vMemory.push_back(pPcb);
  machine.ram.vMemoryLookup[pPcb->name] = machine.ram.vMemory.size() - 1;

  // memory allocation //
  for (int i = pPcb->pLoadAddress; i <= pPcb->pLoadAddress + pPcb->pSize; i++) {
    machine.ram.mem[i][1] = 1;
  }

  // printf("[OS][SCHEDULER][ALLOCATE] Id = %d\n", pPcb->pId);
  // printf("[OS][SCHEDULER][ALLOCATE] state = %d\n", pPcb->pState);

  currentIdCount += 1;
  return pPcb;
}

Pcb *Scheduler::getPcb(std::string filePath) {
  // find pcb using vmem lookup and return ptr to it //
  int index;
  try {
    index = machine.ram.vMemoryLookup.at(filePath);
  } catch (std::out_of_range) {
    printf("[SCH][ERROR][302] --attempted vMemoryLookup.at() with false "
           "program\n");
    return NULL;
  }

  return machine.ram.vMemory[index];
}

int Scheduler::getCurrentPcbId() {
  // get active pcb id //
  if (pPcb != NULL)
    return pPcb->pId;

  // if null send back error code //
  return -1;
}

void Scheduler::queuePcb(Pcb &process) {
  // place pcb into scheduling queues //
  newQueue.push(&process);
  process.updateState(NEW);
}

bool Scheduler::empty() {
  // check if scheduler queues are empty //
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
  // load pcb contents into cpu //
  machine.PC = process.pc;
  machine.Z = process.z;

  for (int i = 0; i <= 6; i++) {
    machine.Reg[i] = process.reg[i];
  }

  printf("[SCH][PRC %d] context to cpu -- pc = %d\n", process.pId, machine.PC);
  return 0;
}

int Scheduler::contextToPcb(Pcb &process) {
  // load contents from cpu to pcb //
  process.pc = machine.PC;
  process.z = machine.Z;
  for (int i = 0; i <= 6; i++) {
    process.reg[i] = machine.Reg[i];
  }
  printf("[SCH][PRC %d] context to pcb -- pc = %d\n", process.pId, machine.PC);

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

  // allow addresses to be overwritten flips bit to zero //
  for (int i = process.pLoadAddress; i <= process.pLoadAddress + process.pSize;
       i++) {
    machine.ram.mem[i][1] = 0;
  }

  // deallocate from vmem and vmemlookup //
  machine.ram.vMemory.erase(machine.ram.vMemory.begin() +
                            machine.ram.vMemoryLookup.at(process.name));
  machine.ram.vMemoryLookup.erase(process.name);

  printf("[SCH][DEALLOC][%s]\n\n", process.name.c_str());
}

// -------------------------
// Interrupt Service Routine
// -------------------------

void Scheduler::interruptServiceRoutine(Pcb &process, int returnCode) {
  printf("[SCH][IRS] -- interrupt service routine started\n");
  switch (returnCode) {
  case PRINT:
    printf("\nprint interrupt\n\n");
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

int Scheduler::fork(Pcb &process) {

  // copy the pcb with context //
  Pcb *cPcb = new Pcb(process);
  // load child ptr into parent //
  process.pChild = cPcb;
  return 0;
}
