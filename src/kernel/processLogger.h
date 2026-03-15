#ifndef PROCESSLOGGER_H
#define PROCESSLOGGER_H

#include "../hardware/vm.h"
#include <iostream>
#include <locale>
#include <stdio.h>

class ProcessLogger {

public:
  std::multimap<int, Pcb *> logMap;
  void logProcess(Pcb &pTerminatedPcb);
  void clearLogMap();
  void printGannt();
  void setVerbosityFlag();

private:
  bool verbosityFlag = false;
};
#endif
