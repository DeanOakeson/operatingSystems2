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
  if (pRunningPcb == NULL && !readyQueuesEmpty()) {
    pRunningPcb = popQueue(READY);
    pRunningPcb->updateState(RUNNING);
    clearCpu();
    contextToCpu(*pRunningPcb);
  }

  // run process in running
  if (pRunningPcb != NULL) {
    returnCode = machine.fetchDecodeExecute(*pRunningPcb);
    pRunningPcb->captureTimeSlice(machine.clock);

  } else {
    machine.idle();
    return 0;
  }

  // swi return
  if (returnCode >= 20) {
    contextToPcb(*pRunningPcb);
    queuePcb(*pRunningPcb, WAITING);
    clearCpu();
    interruptServiceRoutine(*pRunningPcb, returnCode);
    pRunningPcb = NULL;
    return returnCode;
  }

  // process termination
  if (returnCode == 10) {
    contextToPcb(*pRunningPcb);
    queuePcb(*pRunningPcb, TERMINATED);
    deallocateMemory(*pRunningPcb);
    pRunningPcb->calculateResponse();
    pRunningPcb->calculateTurnAround();
    if (verbosityFlag == true) {
      printf("[SC]::fcfs - %s is terminating\n", pRunningPcb->name.c_str());
    }
    pRunningPcb = NULL;
    return returnCode;
  }

  // DEBUG//
  return 0;
}

int Scheduler::roundRobin(int quantum) {
  // ROUND ROBIN //
  // must be wrapped in a loop to run a program //
  // each function call advances the clock one step //
  // SEG FAULT

  int returnCode = 0;

  // SEND A NEW PCB TO READY
  while (!newQueue.empty()) {
    queuePcb(*popQueue(NEW), READY);
  }

  // if running queue is empty process prep
  if (pRunningPcb == NULL && !allQueuesEmpty()) {
    pRunningPcb = popQueue(READY);
    clearCpu();
    contextToCpu(*pRunningPcb);
  }

  // if quantum is up context swap
  if (quantumClock % quantum == 0 && !readyQueue0.empty()) {
    contextToPcb(*pRunningPcb);
    queuePcb(*pRunningPcb, READY);
    pRunningPcb = popQueue(READY);
    pRunningPcb->updateState(RUNNING);
    contextToCpu(*pRunningPcb);
  }

  // run process marked as running
  if (pRunningPcb != NULL) {
    returnCode = machine.fetchDecodeExecute(*pRunningPcb);
    pRunningPcb->captureTimeSlice(machine.clock);
    quantumClock += 1;

    if (verbosityFlag == true) {
    }
  } else {
    machine.idle();
    quantumClock += 1;

    return 0;
  }

  // swi return
  if (returnCode >= 20) {
    contextToPcb(*pRunningPcb);
    queuePcb(*pRunningPcb, WAITING);
    clearCpu();
    interruptServiceRoutine(*pRunningPcb, returnCode);
    pRunningPcb = NULL;
    return returnCode;
  }

  // process termination
  if (returnCode == 10) {
    contextToPcb(*pRunningPcb);
    queuePcb(*pRunningPcb, TERMINATED);
    deallocateMemory(*pRunningPcb);
    pRunningPcb->calculateResponse();
    pRunningPcb->calculateTurnAround();
    if (verbosityFlag == true) {
      printf("[SC]::roro - %s is terminating\n", pRunningPcb->name.c_str());
    }
    pRunningPcb = NULL;
    return returnCode;
  }

  return 0;
}

int Scheduler::multiLevelFeedbackQueue(int quantum, int scaler) {
  int returnCode = 0;
  int quantum0 = quantum;
  int quantum1 = quantum * scaler;
  int quantum2 = quantum * 1000;
  int quantumSet;

  // SEND A NEW PCB TO READY
  while (!newQueue.empty()) {
    queuePcb(*popQueue(NEW), READY);
  }

  ////////////////////////
  // PREPERATION PHASE //
  //////////////////////

  // if running queue is empty process prep
  if (pRunningPcb == NULL && !readyQueuesEmpty()) {
    pRunningPcb = popQueue(READY);
    clearCpu();
    contextToCpu(*pRunningPcb);
  }

  // IMPLEMENT A VARIABALE QUANTUM
  //  if quantum is up context swap

  if (pRunningPcb != NULL) {
    switch (pRunningPcb->pPriority) {
    case 0:
      quantumSet = quantum0;
      break;
    case 1:
      quantumSet = quantum1;
      break;
    case 2:
      quantumSet = quantum2;
      break;
    }
  }

  if (quantumClock % quantumSet == 0 && !readyQueuesEmpty()) {
    pRunningPcb->incrementQuatumCount();
    contextToPcb(*pRunningPcb);
    queuePcb(*pRunningPcb, READY);
    pRunningPcb = popQueue(READY);
    pRunningPcb->updateState(RUNNING);
    contextToCpu(*pRunningPcb);
  }

  // run process marked as running
  if (pRunningPcb != NULL) {
    returnCode = machine.fetchDecodeExecute(*pRunningPcb);
    pRunningPcb->captureTimeSlice(machine.clock);
    quantumClock += 1;
    if (verbosityFlag == true) {
    }
  } else {
    machine.idle();
    return 0;
  }

  // BE SURE THAT EVERTHING GETS BACK TO THE RIGHT READY QUEUE AFTER INTERUPT
  //  swi return
  if (returnCode >= 20) {
    contextToPcb(*pRunningPcb);
    queuePcb(*pRunningPcb, WAITING);
    clearCpu();
    interruptServiceRoutine(*pRunningPcb, returnCode);
    pRunningPcb->decrementQuantumCount();
    pRunningPcb = NULL;
    return returnCode;
  }

  // process termination
  if (returnCode == 10) {
    if (verbosityFlag == true) {
      std::cout << "readyQueue0 = " << readyQueue0.size() << "|";
      std::cout << "readyQueue1 = " << readyQueue1.size() << "|";
      std::cout << "readyQueue2 = " << readyQueue0.size() << "|" << std::endl;
    }
    contextToPcb(*pRunningPcb);
    queuePcb(*pRunningPcb, TERMINATED);
    deallocateMemory(*pRunningPcb);
    pRunningPcb->calculateResponse();
    pRunningPcb->calculateTurnAround();
    if (verbosityFlag == true) {
      printf("[SC]::mlfq - %s is terminating\n", pRunningPcb->name.c_str());
    }
    pRunningPcb = NULL;
    return returnCode;
  }

  return 0;
}

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
    newQueue.push_back(&process);
    break;
  case (READY):
    if (process.pPriority == 0) {
      readyQueue0.push_back(&process);
      break;
    }
    if (process.pPriority == 1) {
      readyQueue1.push_back(&process);
      break;
    }
    if (process.pPriority == 2) {
      readyQueue0.push_back(&process);
      break;
    }
  case (WAITING):
    waitingQueue.push_back(&process);
    break;
  case (TERMINATED):
    terminatedQueue.push(&process);
    break;
  }
  process.updateState(queue);
}

Pcb *Scheduler::popQueue(int queue) {

  std::deque<Pcb *> *readyQueues[] = {&readyQueue0, &readyQueue1, &readyQueue2};
  Pcb *pPcb = NULL;
  switch (queue) {
  case (NEW):
    pPcb = newQueue.front();
    newQueue.pop_front();
    return pPcb;
  case (READY):
    for (auto *deq : readyQueues)
      if (!deq->empty()) {
        pPcb = deq->front();
        deq->pop_front();
        break;
      }
    break;
  case (WAITING):
    pPcb = waitingQueue.front();
    waitingQueue.pop_front();
    return pPcb;
  case (TERMINATED):
    pPcb = terminatedQueue.front();
    terminatedQueue.pop();
    return pPcb;
  }
  return pPcb;
}

bool ::Scheduler::readyQueuesEmpty() {
  if (readyQueue0.empty() && readyQueue1.empty() && readyQueue2.empty()) {
    return true;
  }
  return false;
}

bool Scheduler::allQueuesEmpty() {
  // check if scheduler queues are empty //
  if (newQueue.empty() && pRunningPcb == NULL && readyQueue0.empty() &&
      readyQueue1.empty() && readyQueue2.empty() && waitingQueue.empty()) {
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
  process.pTerminationTime = machine.clock;
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
      isrPrintString(process);
      break;
      // CHAR PRINT
    default:
      isrPrintChar(process);
      break;
    }

    queuePcb(*popQueue(WAITING), READY);
    break;
  case HALT:
    if (verbosityFlag == true) {
      printf("[SC]::insr - halt\n");
    }
    queuePcb(*popQueue(WAITING), TERMINATED);
    deallocateMemory(*pRunningPcb);
    pRunningPcb->calculateResponse();
    pRunningPcb->calculateTurnAround();
    return;
  case FORK: {
    if (verbosityFlag == true) {
      printf("[SC]::insr - fork\n");
    }
    isrVFork(process);
    queuePcb(*popQueue(WAITING), READY);
    break;
  }
  case WAIT:
    if (verbosityFlag == true) {
      printf("[SC]::insr - wait\n");
    }
    queuePcb(*popQueue(WAITING), READY);
    break;
  case INPUT:
    if (verbosityFlag == true) {
      printf("[SC]::insr - input\n");
    }
    queuePcb(*popQueue(WAITING), READY);
    break;
  }

  return;
}

int Scheduler::isrVFork(Pcb &process) {

  // copy the pcb with context //
  Pcb *child = new Pcb(process);
  // load child ptr into parent //
  process.pChild = child;
  machine.ram.vMemory.push_back(child);
  machine.ram.vMemoryLookup[child->name] = machine.ram.vMemory.size() - 1;
  child->pArrivalTime = machine.clock;
  currentIdCount += 1;
  // sets z to 1 to indicate that it is the child program
  child->Z = 1;
  queuePcb(*child, READY);
  return 0;
}

int Scheduler::isrWait(Pcb &process) {}

int Scheduler::isrRandomNumberGen() {}

int Scheduler::isrInputChar() {}

int Scheduler::isrPrintString(Pcb &process) {
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

int Scheduler::isrPrintChar(Pcb &process) {

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
