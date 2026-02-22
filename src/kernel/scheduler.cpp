#include "scheduler.h"
// -----------
// Constructor
// -----------

Scheduler::Scheduler(VirtualMachine &machine) : machine(machine) {}

// ----------------
// Scheduling algos
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
      if (!readyQueue.empty() &&
          pPcb->pArrivalTime <
              static_cast<Pcb *>(readyQueue.front())->pArrivalTime) {
        readyQueue.push_front(pPcb);
        break;
      }
      readyQueue.push_back(pPcb);
    }

    // WHILE READY QUEUE IS NOT EMPTY
    while (!readyQueue.empty()) {
      // PROCESS PREP
      pPcb = static_cast<Pcb *>(readyQueue.front());
      if (machine.clock < pPcb->pArrivalTime) {
        setClock(*pPcb);
      }
      pPcb->updateState(RUNNING);
      runningQueue.push(pPcb);
      readyQueue.pop_front();

      // PROCESS RUN
      returnCode = runProgram(*pPcb);

      // SWI RETURN
      if (returnCode >= 20) {
        pPcb->updateState(WAITING);
        runningQueue.pop();
        waitingQueue.push(pPcb);
        contextToPcb(*pPcb);
        clearCpu();
        interruptServiceRoutine(*pPcb, returnCode);
        printf("[OS][SCHEDULER] -- interrupt handeled\n");
        pPcb->pc += 6;
        waitingQueue.pop();
        if (!readyQueue.empty() &&
            pPcb->pArrivalTime <=
                static_cast<Pcb *>(readyQueue.front())->pArrivalTime) {
          readyQueue.push_front(pPcb);
          pPcb->updateState(READY);

          break;
        }
        readyQueue.push_back(pPcb);
        pPcb->updateState(READY);
        break;
      }

      // PROCESS TERMINATION
      if (returnCode == 0) {
        pPcb->updateState(TERMINATED);
        runningQueue.pop();
        terminatedQueue.push(pPcb);
        deallocateMemory(*pPcb);
        printf("[OS][SCHEDULER] prc %s is terminating\n\n", pPcb->name.c_str());

        terminatedQueue.pop();
        delete pPcb;
        break;
      }
    }
    printf("[OS][SCHEDULER] FCFS pass = %d\n", passNumber);
  }
  return 0;
}

// ------------------
// Single Program Run
// ------------------

int Scheduler::runProgram(Pcb &process) {

  clearCpu();
  contextToCpu(process);
  int returnCode = machine.runCpu(process);
  return returnCode;
}

// --------------
// PCB Managment
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

int Scheduler::setClock(Pcb &process) {
  process.pArrivalTime = machine.clock;
  return 0;
}

void Scheduler::deallocateMemory(Pcb &process) {
  // FLIPS MEM OCCUPATION BIT AT EACH ADDRESS BACK TO ZERO
  for (int i = process.pLoadAddress; i <= process.pLoadAddress + process.pSize;
       i++) {
    machine.ram.mem[i][1] = 0;
  }
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
