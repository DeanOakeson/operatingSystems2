#include "scheduler.h"
// -----------
// Constructor
// -----------

Scheduler::Scheduler(VirtualMachine &machine) : machine(machine) {}

void Scheduler::setVerbosityFlag() {
  if (verbosityFlag == true) {
    verbosityFlag = false;
    return;
  }

  verbosityFlag = true;
  return;
}
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
    queuePcb(*popQueue(NEW), READY);
  }

  // if running queue is empty process prep
  if (runningQueue.empty() && !readyQueue.empty()) {
    pRunningPcb = popQueue(READY);
    queuePcb(*pRunningPcb, RUNNING);
    clearCpu();
    contextToCpu(*pRunningPcb);
  }

  // run process in running
  if (pRunningPcb != NULL) {
    returnCode = machine.fetchDecodeExecute(*pRunningPcb);
    pRunningPcb->cpuTimeSlices.push_back(machine.clock);
  } else {
    machine.idle();
    return 0;
  }

  // swi return
  if (returnCode >= 20) {
    popQueue(RUNNING);
    queuePcb(*pRunningPcb, WAITING);
    contextToPcb(*pRunningPcb);
    clearCpu();
    interruptServiceRoutine(*pRunningPcb, returnCode);
    if (verbosityFlag == true) {
      printf("[SC]::fcfs -- interrupt handeled\n");
    }
    waitingQueue.pop();
    queuePcb(*pRunningPcb, READY);
    return returnCode;
  }

  // process termination
  if (returnCode == 10) {
    popQueue(RUNNING);
    contextToPcb(*pRunningPcb);
    queuePcb(*pRunningPcb, TERMINATED);
    if (verbosityFlag == true) {
      printf("[SC]::fcfs - %s is terminating\n", pRunningPcb->name.c_str());
    }
    deallocateMemory(*pRunningPcb);
    pRunningPcb = NULL;
    return returnCode;
  }

  // DEBUG//
  if (verbosityFlag == true) {
    printf("[SC]::fcfs clock = %d\n", machine.clock);
  }
  return 0;
}

int Scheduler::roundRobin() {}

// --------------
// PCB and QUEUE Managment
// -------------

Pcb *Scheduler::createPcb(std::vector<int> asmHeader, std::string filePath) {
  Pcb *pPcb = new Pcb(asmHeader, filePath);
  pPcb->pId = currentIdCount;

  // PUSH ONTO vMEMORY and vMEMORY LOOKUP
  machine.ram.vMemory.push_back(pPcb);
  machine.ram.vMemoryLookup[pPcb->name] = machine.ram.vMemory.size() - 1;
  currentIdCount += 1;
  return pPcb;
}

Pcb *Scheduler::getPcb(std::string filePath) {
  // find pcb using vmem lookup and return ptr to it //
  int index;
  try {
    index = machine.ram.vMemoryLookup.at(filePath);
  } catch (std::out_of_range) {
    printf("[SC]::gPcb - ERROR/302 - attempted vMemoryLookup.at() with false "
           "program\n");
    return NULL;
  }

  return machine.ram.vMemory[index];
}

void Scheduler::queuePcb(Pcb &process, int queue) {
  // place pcb into scheduling queues //
  switch (queue) {
  case (NEW):
    newQueue.push(&process);
    break;
  case (READY):
    readyQueue.push_back(&process);
    break;
  case (WAITING):
    waitingQueue.push(&process);
    break;
  case (RUNNING):
    runningQueue.push(&process);
    break;
  case (TERMINATED):
    terminatedQueue.push(&process);
    break;
  }
  process.updateState(queue);
}

Pcb *Scheduler::popQueue(int queue) {

  Pcb *pPcb;
  switch (queue) {
  case (NEW):
    pPcb = newQueue.front();
    newQueue.pop();
    return pPcb;
  case (READY):
    pPcb = readyQueue.front();
    readyQueue.pop_front();
    return pPcb;
  case (WAITING):
    pPcb = waitingQueue.front();
    waitingQueue.pop();
    return pPcb;
  case (RUNNING):
    pPcb = runningQueue.front();
    runningQueue.pop();
    return pPcb;
  case (TERMINATED):
    pPcb = terminatedQueue.front();
    terminatedQueue.pop();
    return pPcb;
  }
  return NULL;
}

bool Scheduler::empty() {
  // check if scheduler queues are empty //
  if (readyQueue.empty() && newQueue.empty() && runningQueue.empty() &&
      waitingQueue.empty()) {
    return true;
  }
  return false;
}

//------------------
// MEMORY MANAGMENT
//-----------------

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
  if (verbosityFlag == true) {

    printf("[SC]::dalo - %s\n", process.name.c_str());
  }
}

int Scheduler::allocateMemory(Pcb &process) {

  // creates pcb and flips the memory allocation bits to 1

  // memory allocation //
  for (int i = process.pLoadAddress; i <= process.pLoadAddress + process.pSize;
       i++) {
    machine.ram.mem[i][1] = 1;
  }
  if (verbosityFlag == true) {
    printf("[SC]::allo - pId = %d\n", process.pId);
    printf("[SC]::allo - pState = %d\n", process.pState);
  }

  return 0;
}

// ----------------
// CONTEXT FUNCTIONS
// ----------------

int Scheduler::contextToCpu(Pcb &process) {
  // load pcb contents into cpu //
  machine.PC = process.PC;
  machine.Z = process.Z;

  for (int i = 0; i <= 6; i++) {
    machine.Reg[i] = process.Reg[i];
  }

  // printf("[OS][SCH][PRC %d] context to cpu - pc = %d\n", process.pId,
  // machine.PC);
  return 0;
}

int Scheduler::contextToPcb(Pcb &process) {
  // load contents from cpu to pcb //
  process.PC = machine.PC;
  process.Z = machine.Z;
  for (int i = 0; i <= 6; i++) {
    process.Reg[i] = machine.Reg[i];
  }
  // printf("[OS][SCH][PRC %d] context to pcb - pc = %d\n", process.pId,
  // machine.PC);

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

// -------------------------
// Interrupt Service Routine
// -------------------------

void Scheduler::interruptServiceRoutine(Pcb &process, int returnCode) {
  // printf("[SCH][IRS] -- interrupt service routine started\n");
  switch (returnCode) {
  case PRINT:
    switch (machine.ram.mem[process.Reg[0]][0]) {
      //'"' STRING PRINT
    case 34:
      irsPrintString(process);
      break;
      // CHAR PRINT
    default:
      irsPrintChar(process);
      break;
    }
    break;
  case HALT:
    printf("halt\n");
    break;
  case FORK: {
    if (verbosityFlag == true) {
      printf("[SC]::irs --fork\n");
    }
    irsVFork(process);
    break;
  }
  case WAIT:
    if (verbosityFlag == true) {
      printf("[SC]::irs --wait\n");
    }
    break;
  case INPUT:
    if (verbosityFlag == true) {
      printf("[SC]::irs --input\n");
    }
    break;
  }
  return;
}

int Scheduler::irsVFork(Pcb &process) {

  // copy the pcb with context //
  Pcb *child = new Pcb(process);
  // load child ptr into parent //
  process.pChild = child;
  machine.ram.vMemory.push_back(child);
  machine.ram.vMemoryLookup[child->name] = machine.ram.vMemory.size() - 1;
  readyQueue.push_back(child);
  process.updateState(READY);
  currentIdCount += 1;
  // sets z to 1 to indicate that it is the child program
  child->Z = 1;
  return 0;
}

int Scheduler::irsWait(Pcb &process) {}

int Scheduler::irsPrintString(Pcb &process) {
  // STRING PRINT
  process.Reg[0] += 1;
  while (machine.ram.mem[process.Reg[0]][0] != 34) {
    switch (machine.ram.mem[process.Reg[0]][0]) {
    case 92: //'\' = newline char
      printf("\n");
      process.Reg[0] += 1;
      break;
    case 36: //'$' = space char
      printf(" ");
      process.Reg[0] += 1;
      break;
    default:
      printf("%c", machine.ram.mem[process.Reg[0]][0]);
      process.Reg[0] += 1;
      break;
    }
  }
  return 0;
}

int Scheduler::irsPrintChar(Pcb &process) {

  switch (machine.ram.mem[process.Reg[0]][0]) {
  case 92: //'\' = newline char
    printf("\n");
    break;
  case 36: //'$' = space char
    printf(" ");
    break;
  default:
    printf("%c", machine.ram.mem[process.Reg[0]][0]);
    break;
  }

  return 0;
}
