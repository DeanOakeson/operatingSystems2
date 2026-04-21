#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../hardware/vm.h"

class Scheduler {
public:
  VirtualMachine &machine;

  //----------
  // PCB States
  //----------

  static const u_int8_t NEW = 1;
  static const u_int8_t READY = 2;
  static const u_int8_t RUNNING = 3;
  static const u_int8_t WAITING = 4;
  static const u_int8_t TERMINATED = 5;

  //--------------------
  // Soft Ware Interrputs
  //--------------------

  static const u_int8_t HALT = 10;
  static const u_int8_t PRINT_INT = 20;
  static const u_int8_t PRINT = 21;
  static const u_int8_t INPUT = 22;
  static const u_int8_t FORK = 23;
  static const u_int8_t RET_SMRP = 24;
  static const u_int8_t SEM_AQ = 25;
  static const u_int8_t SEM_RE = 26;
  static const u_int8_t SEM_WAIT = 27;
  static const u_int8_t WAIT = 28;

public:
  std::queue<Pcb *> terminatedQueue;

  Scheduler(VirtualMachine &machine);
  void setVerbosityFlag();

  int allocateMemory(Pcb &process);
  void deallocateMemory(Pcb &process);
  void deallocateShMemory(Pcb &process);
  Pcb *createPcb(std::vector<int> asmHeader, std::string filePath);

  int initSemaphore(int value);

  void queuePcb(Pcb &process, int queue);
  Pcb *popQueue(int queue);

  bool readyQueuesEmpty();
  bool allQueuesEmpty();

  int firstComeFirstServe();
  int roundRobin(int quantum);
  int multiLevelFeedbackQueue(int quantum, int scaler);

private:
  struct Semaphore {
    int sId;
    int value;
    std::deque<Pcb *> blockQueue;
  };

  std::vector<Semaphore *> semaphoreTable;

  std::deque<Pcb *> newQueue;
  std::deque<Pcb *> waitingQueue;
  std::deque<Pcb *> readyQueue0, readyQueue1, readyQueue2;

  bool verbosityFlag = false;

  Pcb *pRunningPcb = NULL;
  int prcIdCounter = 0;
  int semIdCounter = 0;
  int quantumClock = 0;

  int contextToCpu(Pcb &process);
  int contextToPcb(Pcb &process);
  int setClock(Pcb &process);
  int clearCpu();

  //------------------------
  // InterruptServiceRoutine
  //-----------------------

  void interruptServiceRoutine(Pcb &process, int returnCode);
  int isrVFork(Pcb &process);
  int isrWait(Pcb &process);
  int isrHalt(Pcb &process);
  int isrRandomNumberGen();
  int isrInputChar();
  int isrPrintString(Pcb &process);
  int isrPrintChar(Pcb &process);
  int isrPrintInt(Pcb &process);
  int isrReturnSharedMemPtr(Pcb &process);
  int isrSemaphoreAquire(Pcb &process);
  int isrSemaphoreRelease(Pcb &process);
  int isrSemaphoreWait(Pcb &process);
};
#endif
