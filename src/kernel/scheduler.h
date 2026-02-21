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

  Scheduler(VirtualMachine &machine);

public:
  int createPcb(std::vector<int> asmHeader, std::string filePath);
  Pcb *getPcb(std::string filePath);
  void updateState(Pcb &process, int state);
  void queuePcb(Pcb &process);

  int runProgram(Pcb &process);
  int firstComeFirstServe();

private:
  std::queue<Pcb *> newQueue;
  std::queue<Pcb *> readyQueue;
  std::queue<Pcb *> runningQueue;
  std::queue<Pcb *> waitingQueue;
  std::queue<Pcb *> terminatedQueue;

  int contextToCpu(Pcb &process);
  int contextToPcb(Pcb &process);
  int clearCpu();

  //------------------------
  // InterruptServiceRoutine
  //-----------------------

  void interruptServiceRoutine(Pcb &process, int returnCode);
};
#endif
