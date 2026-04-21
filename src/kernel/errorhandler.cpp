#include "errorhandler.h"

ErrorHandler::ErrorHandler(VirtualMachine &machine) : machine(machine) {}
void ErrorHandler::setVerbosityFlag() {
  if (verbosityFlag == true) {
    verbosityFlag = false;
    return;
  }

  verbosityFlag = true;
}

// IF IT RECIEVES -1 THEN IT RETURNS THE FINAL ELEMENT OF THE LIST
ProcessLog *ErrorHandler::getCpuLog(int index) {
  if (!cpuLogs.empty()) {
    if (index == -1) {
      return cpuLogs.back();
    }
    return cpuLogs.at(index);
  }
  return NULL;
}

void ErrorHandler::logTerminatedProcesses(std::queue<Pcb *> &terminatedQueue,
                                          std::vector<int *> kernelData) {

  ProcessLog *newProcessLog = new ProcessLog();

  while (!terminatedQueue.empty()) {
    newProcessLog->logProcess(*terminatedQueue.front(), cpuLogCount);
    terminatedQueue.pop();
  }
  cpuLogs.push_back(newProcessLog);
  newProcessLog->calculateAverages();
  newProcessLog->algoUsed = *kernelData[0];
  newProcessLog->quantum = *kernelData[1];
  newProcessLog->ratio = *kernelData[2];
  cpuLogCount++;
  if (verbosityFlag == true)
    printf("[EH]::ltep - all proceesses logged succesfully\n");
  return;
}

int ErrorHandler::errorDump() {
  if (errorList.empty()) {
    return 1;
  }
  std::cout << "[OS]::errh/errorDump\n"
               "====================\n";
  for (const int &i : errorList) {
    switch (i) {
    case CPU_EARLY_TERMINATION:
      std::cout << " [LD]:: ldpr -100 --cpu early termination--" << std::endl;
      break;
    case MEM_OVERFLOW:
      std::cout << "[LD]::ldpr -201 --memory overflow----" << std::endl;
      break;
    case MEM_OVERWRITE:
      std::cout << "[LD]::ldpr -202 --attempted memory overwrite" << std::endl;
      break;
    case NOT_SUFFICIENT_MEM:
      std::cout
          << "[LD]::ashm 204 --not enough memory to allocate shared memory"
          << std::endl;
      break;
    case LOAD_FILE_NOT_FOUND:
      std::cout << "[LD]::ldpr -200 --file not found--" << std::endl;
      break;
    case MEM_DUMP_NO_PROGRAM:
      std::cout << "[EH]::mdmp -301 --attempted memDump() no loaded program"
                << std::endl;
      break;
    case FALSE_PROGRAM_LOOKUP:
      std::cout << "[VM]::gpcb -302 --attempted getPcb() with false program "
                << std::endl;
      break;
    case CORE_DUMP_NO_PRCLOG:
      std::cout << " [EH]::cdmp -303 --attempted coreDump() no process log"
                << std::endl;
    }
  }
  return 0;
}

int ErrorHandler::memDumpEveryAddress() {
  u_long ratio = 8;
  u_long offset = 6;
  for (u_long i = 0; i < MEM_SIZE_KB / ratio; i += offset) {
    // ADDRESS
    std::cout << "[" << std::hex << std::setw(4) << std::setfill('0') << i
              << "]: ";
    // HEX bytes - row 1
    for (u_long j = 0; j < offset; j++) {
      if (i + j < MEM_SIZE_KB / 2)
        std::cout << std::hex << std::setw(2)
                  << static_cast<int>(machine.ram.mem[i + j][0]) << " "
                  << static_cast<int>(machine.ram.mem[i + j][1]) << " ";
      else
        std::cout << "   ";
    }
    std::cout << "\n";
  }
  return 0;
}

int ErrorHandler::memDump(std::string filePath) {
  int offset = 6;
  Pcb *pPcb = machine.ram.getPcb(filePath);
  if (pPcb == NULL) {
    return 204;
  }

  Shm *pShm = pPcb->sharedMemory;
  std::cout << "MEMORY LOCATIONS: " << pPcb->name << std::endl;
  for (int i = pPcb->pLoadAddress; i < pPcb->pEndAddress; i += offset) {
    // ADDRESS
    std::cout << "[" << std::hex << std::setw(4) << std::setfill('0') << i
              << "]: ";
    // HEX bytes - row 1
    for (int j = 0; j < offset; j++) {
      if (i + j < pPcb->pEndAddress)
        std::cout << std::hex << std::setw(2)
                  << static_cast<int>(machine.ram.mem[i + j][0]) << " "
                  << static_cast<int>(machine.ram.mem[i + j][1]) << " ";
      else
        std::cout << "   ";
    }
    std::cout << std::dec << "\n";
  }
  if (pShm != NULL) {
    std::cout << std::setw(4) << std::setfill('_')
              << "SHARED MEMORY: " << pShm->sId << std::endl;
    for (int i = pShm->sLoadAddress; i < pShm->sEndAddress; i += offset) {
      // ADDRESS
      std::cout << "[" << std::hex << std::setw(4) << std::setfill('0') << i
                << "]: ";
      // HEX bytes - row 1
      for (int j = 0; j < offset; j++) {
        if (i + j < pShm->sEndAddress)
          std::cout << std::hex << std::setw(2)
                    << static_cast<int>(machine.ram.mem[i + j][0]) << " "
                    << static_cast<int>(machine.ram.mem[i + j][1]) << " ";
        else
          std::cout << "   ";
      }
      std::cout << "\n";
    }
  }
  return 0;
}
int ErrorHandler::memDumpAll() {

  // CHECK IF THERE ARE ANY PCBS LOADED IN VMEM
  // IF NO FILE HAS BEEN LOADED
  if (machine.ram.vMemory.size() == 0) {
    if (verbosityFlag == true) {
      printf("[EH]::mdmp -301 --attempted memDump() with no loaded program\n");
    }
    return 301; // 1 memDump means no load
  }

  printf("\n[EH]::mdma \n===========\n");
  Pcb *pPcb;
  // this iterates over the actual vMemory not the hash
  for (auto it = machine.ram.vMemory.begin(); it != machine.ram.vMemory.end();
       it++) {
    pPcb = *it;
    memDump(pPcb->name);
  }

  return 0;
}

void ErrorHandler::coreDump(ProcessLog &processLog) {
  int globalEnd = 0;
  int colorInt = 41; // START OF COLOR GAMMUT
  int globalStart = INT_MAX;

  std::string colorModifier = "\033[30;";

  // COLOR MAP POPULATION
  for (auto cIt = processLog.ganntChart.begin();
       cIt != processLog.ganntChart.end(); cIt++) {
    auto foundColor = colorMap.find(cIt->second);
    if (foundColor == colorMap.end()) {

      std::string colorInsert = "";
      colorInsert.append(colorModifier);
      colorInsert.append(std::to_string(colorInt));
      colorInsert.append("m");

      colorMap.insert({cIt->second, colorInsert});
      colorInt++;
    }
  }

  // FIND GLOBAL START AND GLOBAL END
  for (auto it = processLog.processMap.begin();
       it != processLog.processMap.end(); it++) {
    Pcb *p = it->second;
    if (!p->cpuTimeSlices.empty()) {
      globalStart = std::min(globalStart, p->pArrivalTime);
      globalEnd = std::max(globalEnd, p->cpuTimeSlices.back());
    }
  }

  // BEGIN PRINTING STATS
  if (!processLog.processMap.empty()) {
    for (auto it = processLog.processMap.begin();
         it != processLog.processMap.end(); it++) {
      Pcb *pPcb = it->second;
      std::string colorMod = colorMap.find(pPcb->pId)->second;
      std::cout << colorMod << " " << pPcb->name << "\033[0m" << std::endl
                << std::endl;

      printf("CHILD ............. =");

      if (pPcb->pChild != NULL) {
        printf(" %s\n", pPcb->pChild->name.c_str());
      } else {
        printf(" NULL\n");
      }

      std::cout << "PRC arrive ........ [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->pArrivalTime
                << "]\n"
                << "PRC terminate ..... [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->pTerminationTime
                << "]\n"
                << "PRC id ............ [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->pId << "]\n"
                << "PRC turnaround .... [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->turnAround
                << "]\n"
                << "PRC response ...... [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->response << "]\n"
                << "PRC wait .......... [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->wait << "]\n\n";

      std::cout << "REG #1 ............ [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->Reg[1] << "]\n"
                << "REG #2 ............ [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->Reg[2] << "]\n"
                << "REG #3 ............ [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->Reg[3] << "]\n"
                << "REG #4 ............ [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->Reg[4] << "]\n"
                << "REG #5 ............ [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->Reg[5] << "]\n"
                << "REG #Z ............ [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->Z << "]\n"
                << "REG PC ............ [" << std::setw(7) << std::setfill(' ')
                << std::fixed << std::setprecision(1) << pPcb->PC << "]\n";

      if (verbosityFlag == true) {
        printPrcGannt(*pPcb, globalStart, globalEnd);
      }
      std::cout << std::endl;
    }
  }

  // PRINT LOG STATS

  std::cout << "\033[1;30;43m" << "  PROCESS LOG #" << processLog.pLogId
            << "\033[0m" << "\n\n";

  std::cout << "LOG id ............ [" << std::setw(7) << std::setfill(' ')
            << std::fixed << std::setprecision(1) << processLog.pLogId << "]\n"
            << "LOG turnaround .... [" << std::setw(7) << std::setfill(' ')
            << std::fixed << std::setprecision(1) << processLog.avgTurnAround
            << "]\n"
            << "LOG response ...... [" << std::setw(7) << std::setfill(' ')
            << std::fixed << std::setprecision(1) << processLog.avgResponse
            << "]\n"
            << "LOG wait .......... [" << std::setw(7) << std::setfill(' ')
            << std::fixed << std::setprecision(1) << processLog.avgWait << "]\n"
            << "LOG algo .......... [" << std::setw(7) << std::setfill(' ')
            << std::fixed << std::setprecision(1) << processLog.algoUsed
            << "]\n"
            << "LOG quantum ....... [" << std::setw(7) << std::setfill(' ')
            << std::fixed << std::setprecision(1) << processLog.quantum << "]\n"
            << "LOG ratio ......... [" << std::setw(7) << std::setfill(' ')
            << std::fixed << std::setprecision(1) << processLog.ratio << "]\n"

            << "\n\n";

  // PRINT LOG GANNT
  printLogGannt(processLog, globalStart, globalEnd);
}

void ErrorHandler::printLogGannt(ProcessLog &processLog, int globalStart,
                                 int globalEnd) {
  // SOMEHTING STRANGE HAPPENS WHEN CALLING COREDUMP A SECOND TIME ON THE SAME
  // DATA
  int newLineCounter = 0;

  std::string currentColor;
  auto it = processLog.ganntChart.begin();
  int label = globalStart;
  for (int i = globalStart; i <= globalEnd; i++) {
    if (newLineCounter % 40 == 0 && newLineCounter != 0) {
      // update the label to be at divsor of 10
      std::cout << "|" << label << " -- ";
      label += 20;
      std::cout << label - 1 << std::endl;
    }

    newLineCounter++;
    if (it->first == i) {
      currentColor = colorMap.find(it->second)->second;
      std::cout << currentColor << "#" << "\033[0m";
      // std::cout << it->second;
      it++;
    } else
      std::cout << "\033[30;47m" << "#" << "\033[0m";
  }
  std::cout << std::endl;
}

void ErrorHandler::printPrcGannt(Pcb &process, int globalStart, int globalEnd) {

  int size = process.cpuTimeSlices.size();

  int j = 0;
  printf("\n");
  int label = globalStart;

  int newLineCounter = 0;
  for (int i = globalStart; i <= globalEnd; i++) {
    // COLUMNS
    if (newLineCounter % 40 == 0 && newLineCounter != 0) {
      std::cout << "|" << label << " -- ";
      label = label + 20;
      std::cout << label - 1 << std::endl;
    }
    newLineCounter++;
    if (j < size && process.cpuTimeSlices[j] == i) {
      if (i == process.pTerminationTime) {
        // BLUE
        std::cout << "\033[30;43m" << "#" << "\033[0m";
      } else {
        // RED
        std::cout << "\033[30;41m" << "#" << "\033[0m";
      }
      j++;

    } else {
      if (i == process.pArrivalTime) {
        // YELLOW
        std::cout << "\033[30;44m" << "#" << "\033[0m";
      } else {

        // WHITE
        std::cout << "\033[30;47m" << "#" << "\033[0m";
      }
    }
  }

  printf("\n");
};
