#include "processLogger.h"
// FIX pID SYSTEM IT IS GETTING CONFUSED

void ProcessLogger::setVerbosityFlag() {
  if (verbosityFlag == true) {
    verbosityFlag = false;
    return;
  }

  verbosityFlag = true;
}

void ProcessLogger::logProcess(Pcb &pTerminatedPcb) {
  if (verbosityFlag == true) {
    printf("[PL]::lprc - pId %d - ", pTerminatedPcb.pId);
    std::cout << pTerminatedPcb.name << "\n";
  }
  logMap.insert(std::make_pair(pTerminatedPcb.pId, &pTerminatedPcb));
}

void ProcessLogger::clearLogMap() { logMap.clear(); }

void ProcessLogger::printGannt() {

  wchar_t graybar = 0Xe283a3;
  auto pcbLogIndex = logMap.begin();

  Pcb *largestPcb = pcbLogIndex->second;
  for (int i = 0; i < logMap.size(); i++) {
  }

  for (int i = 0; i < logMap.size(); i++) {
    int pId = pcbLogIndex->first;
    Pcb *tPcb = pcbLogIndex->second;
    int start = tPcb->cpuTimeSlices[0];
    int end = tPcb->cpuTimeSlices.back();

    printf("[PL]::gant - %d - ", pId);

    int j = 0;
    for (int i = 0; i < end; i++) {
      if (tPcb->cpuTimeSlices[j] == i) {
        printf("# ");
        j++;
      } else {
        printf("  ");
      }
    }

    if (verbosityFlag == true) {
      printf("\n[PL]::gant - %d - ", pId);
      for (int i = 0; i < tPcb->cpuTimeSlices.size(); i++) {
        printf("%d ", tPcb->cpuTimeSlices[i]);
      }
    }

    printf("\n");
    pcbLogIndex++;
  }
};
