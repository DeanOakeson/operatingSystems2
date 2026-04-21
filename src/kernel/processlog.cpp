#include "processlog.h"

void ProcessLog::logProcess(Pcb &terminatedProcess, int idCount) {
  processMap.insert({terminatedProcess.pId, &terminatedProcess});
  pLogId = idCount;

  for (int i = 0; i < terminatedProcess.cpuTimeSlices.size(); i++) {
    ganntChart.insert(
        {terminatedProcess.cpuTimeSlices[i], terminatedProcess.pId});
  }
  return;
}
