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

  static const u_int8_t PRINT = 21;
  static const u_int8_t HALT = 22;
  static const u_int8_t FORK = 23;
  static const u_int8_t WAIT = 24;
  static const u_int8_t INPUT = 25;

public:
  std::queue<Pcb *> terminatedQueue;

  Scheduler(VirtualMachine &machine);
  void setVerbosityFlag();

  int allocateMemory(Pcb &process);
  Pcb *createPcb(std::vector<int> asmHeader, std::string filePath);
  Pcb *getPcb(std::string filePath);

  void queuePcb(Pcb &process, int queue);
  Pcb *popQueue(int queue);
  bool empty();

  int firstComeFirstServe();
  int roundRobin();
  int multiLevelFeedbackQueue();

private:
  std::queue<Pcb *> newQueue;
  std::deque<Pcb *> readyQueue;
  std::queue<Pcb *> runningQueue;
  std::queue<Pcb *> waitingQueue;

  bool verbosityFlag = false;

  Pcb *pRunningPcb = NULL;
  int currentIdCount = 0;

  void deallocateMemory(Pcb &process);
  int contextToCpu(Pcb &process);
  int contextToPcb(Pcb &process);
  void updateState(Pcb &process, int state);
  int setClock(Pcb &process);
  int clearCpu();

  //------------------------
  // InterruptServiceRoutine
  //-----------------------

  void interruptServiceRoutine(Pcb &process, int returnCode);
  int irsVFork(Pcb &process);
  int irsWait(Pcb &process);
  int irsPrintString(Pcb &process);
  int irsPrintChar(Pcb &process);
};
#endif
