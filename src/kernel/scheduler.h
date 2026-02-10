#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../hardware/vm.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <list>
#include <stdio.h>
#include <stdlib.h>

class Scheduler {
public:
  VirtualMachine &machine;
  static const u_int8_t NEW = 1;
  static const u_int8_t READY = 2;
  static const u_int8_t RUNNING = 3;
  static const u_int8_t WATING = 4;
  static const u_int8_t TERMINATED = 5;

  Scheduler(VirtualMachine &machine);

  std::queue<Pcb> newQueue;
  std::queue<Pcb> readyQueue;
  std::queue<Pcb> runningQueue;
  std::queue<Pcb> waitingQueue;
  std::queue<Pcb> terminatedQueue;

  void updateState(Pcb &process, int state);
  int firstComeFirstServe();
  int runProgram();
};
#endif
