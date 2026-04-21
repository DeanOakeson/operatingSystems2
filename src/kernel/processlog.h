#ifndef PROCESSLOGGER_H
#define PROCESSLOGGER_H

#include "../hardware/vm.h"
#include <iostream>
#include <locale>
#include <stdio.h>

class ProcessLog {

public:
  std::string algoUsed = "MLFQ";
  int pLogId = 0;
  int firstInstructionTime = 0;
  int lastInstructionTime = 0;
  int quantum = 0;
  int ratio = 0;
  float cpuRuntimeMiliSeconds = 0;
  float avgWait;
  float avgResponse;
  float avgTurnAround;
  std::multimap<int, Pcb *> processMap;
  std::multimap<int, int> ganntChart;

  void logProcess(Pcb &pTerminatedPcb, int idCount);
  // WRONG
  void calculateAverages() {
    for (auto it = processMap.begin(); it != processMap.end(); it++) {
      Pcb *pPcb = it->second;
      avgWait += pPcb->wait;
      avgResponse += pPcb->response;
      avgTurnAround += pPcb->turnAround;
    }
    avgWait = avgWait / processMap.size();
    avgResponse = avgResponse / processMap.size();
    avgTurnAround = avgTurnAround / processMap.size();
  };
};
#endif
