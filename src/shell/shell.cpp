#include "shell.h"
// inits my unordered_map and path to vm

Shell::Shell(Kernel &kernel) : kernel(kernel) {}

int Shell::shellVerbose(std::vector<std::string> argList) {
  kernel.setVerbosityFlag();
  if (verbosityFlag == true) {
    verbosityFlag = false;
    return 0;
  }
  verbosityFlag = true;

  std::cout << "[SH]::verb - verbose flag set to '" << verbosityFlag << "'\n";

  return 0;
}

void Shell::initPath() {
  functionMap["run"] = &Shell::shellRun;
  functionMap["load"] = &Shell::shellLoad;
  functionMap["exit"] = &Shell::shellExit;
  functionMap["help"] = &Shell::shellHelp;
  functionMap["clear"] = &Shell::shellClear;
  functionMap["cdmp"] = &Shell::shellCoreDump;
  functionMap["edmp"] = &Shell::shellErrorDump;
  functionMap["mdmp"] = &Shell::shellMemDump;
  functionMap["exec"] = &Shell::shellExecute;
  functionMap["gant"] = &Shell::shellGannt;
  functionMap["-v"] = &Shell::shellVerbose;
  functionMap["test"] = &Shell::shellTest;
  functionMap["sets"] = &Shell::shellSetScheduler;
  functionMap["wout"] = &Shell::shellWriteOut;
  functionMap["oshm"] = &Shell::shellOpenSharedMemory;
  functionMap["insm"] = &Shell::shellInitSemaphore;
}

//////////////////////////////////////
// INPUT ARGUMENT TO ARGUMENT ARRAY //
//////////////////////////////////////

std::vector<std::string>
Shell::commandStringToArrayOfStrings(std::string commandString) {
  std::vector<std::string> commandArray;
  std::istringstream strstm(commandString);
  std::string token;
  while (strstm >> token) {
    commandArray.push_back(token);
  }
  return commandArray;
}

///////////////
// MAIN LOOP //
///////////////

int Shell::shellLoop() {

  while (true) {
    std::cout << "shell --> ";
    std::string command;
    std::getline(std::cin, command);

    std::vector<std::string> commandArray =
        commandStringToArrayOfStrings(command);

    if (functionMap.find(commandArray[0]) == functionMap.end()) {
      // IF FUNCTION IS NOT FOUND IN THE LIST PRINT AND RUN LOOP
      std::cout << "try 'help'\n";
      continue;
    }

    int returnCode = (this->*functionMap[commandArray[0]])(commandArray);
    if (returnCode == 9) { // EXIT CODE
      return 0;
    }
  }
}

//////////////////////////////
// CONSOLE CONTROL PROGRAMS //
//////////////////////////////

int Shell::shellClear(std::vector<std::string> argList) {
  system("clear");
  return 0;
}

int Shell::shellHelp(std::vector<std::string> argList) {
  std::cout
      << "[SH]::help\n"
         "=================================================================\n"
         "run [-v] ------ runs a loaded specified program\n"
         "load [-v]------ loads a binary file from an input file path\n"
         "clear --------- clears the terminal screen\n"
         "clear --------- clears the terminal screen\n"
         "coredump ------ lists the current values contained in REGISTERS\n"
         "errordump ----- prints logged errors\n"
         "exit ---------- exits terminal\n"
         "=================================================================\n";
  return 0;
}

int Shell::shellExit(std::vector<std::string> argList) {
  if (verbosityFlag == true) {
    printf("[SH]::exit\n");
  }
  return 9;
}

/////////////////////////////
// MEMORY CONTROL PROGRAMS //
/////////////////////////////

int Shell::shellLoad(std::vector<std::string> argList) {

  int returnCode;

  switch (argList.size()) {
  case 1:
    printf("[SH]::load - no file provided\n");
    return 1;
  default:
    // load ...
    auto it = argList.begin();
    it++;
    for (auto it = argList.begin() + 1; it != argList.end(); it++) {
      returnCode = kernel.kernelLoadProgram(*it);
      if (returnCode == 1) {
        std::cout << "[SH]::load - attemted load failed " << *it << std::endl;
        continue;
      }
      std::cout << "[SH]::load - [" << *it << "] loaded" << std::endl;
    }
    return 0;
  }
}

int Shell::shellErrorDump(std::vector<std::string> argList) {
  int returnCode;
  switch (argList.size()) {
  case 1:
    returnCode = kernel.kernelErrorDump();
    if (returnCode == 1) {
      printf("[SH]::edmp - no logged errors\n");
    }
    return 0;
  }
  printf("[SH]::load - too many arguments\n");
  return 1;
}

int Shell::shellRun(std::vector<std::string> argList) {

  switch (argList.size()) {
    // run
  case 1:
    kernel.kernelRun();
    return 0;

  case 2:
    // run -v
    if (argList[1] == "-v") {
      kernel.kernelRun();
      kernel.kernelCoreDump();
      return 0;
    }
    printf("[SH]::run %s is not a valid argument\n", argList[1].c_str());
    return 1;
  }
  printf("[SH]::run - not valid usage\n");
  return 1;
}

int Shell::shellExecute(std::vector<std::string> argList) {
  std::multimap<int, std::string> argMap;
  std::multimap<int, std::string> returnMap;

  // argList[0] is the command name ("exec"), so start at index 1
  // Format: <program> [arrivalTime] <program> [arrivalTime] ...
  // If no arrival time follows a program, default to 0

  if (argList.size() == 1) {
    printf("[SH]::exec - no file provided\n");
  }

  int i = 1; // skip the command name
  while (i < argList.size()) {
    std::string program = argList[i];
    i++;

    // Check if the next argument is an arrival time (a number)
    int arrivalTime = 0;
    if (i < argList.size()) {
      try {
        arrivalTime = stoi(argList[i]);
        i++; // consume the number
      } catch (const std::invalid_argument &) {
        arrivalTime = 0; // next arg is another program, not a time
      }
    }

    argMap.insert({arrivalTime, program});
  }

  if (verbosityFlag == true) {
    for (auto &[time, prog] : argMap) {
      printf("[SH]::exec - time = %d", time);
      printf(", prog = %s\n", prog.c_str());
    }
  }

  returnMap = kernel.kernelExecuteProgram(argMap);

  for (auto it = returnMap.begin(); it != returnMap.end(); ++it) {
    if (it->first == 1)
      printf("[SH]::exec - failed to load '%s'\n", it->second.c_str());
  }

  return 0;
}

int Shell::shellGannt(std::vector<std::string> argList) {

  switch (argList.size()) {

  case 1:
    kernel.kernelPrintGanntChart();
    return 0;
  }
  printf("[SH]::gant - not valid usage\n");
  return 1;
}

/////////////////////
// ERROR MANAGMENT //
/////////////////////

int Shell::shellCoreDump(std::vector<std::string> argList) {
  int returnCode;
  returnCode = kernel.kernelCoreDump();
  if (returnCode == 1) {
    printf("[SH]::cdmp - kernelCoreDump() failed\n");
  }
  return 0;
}

int Shell::shellMemDump(std::vector<std::string> argList) {
  int returnCode = 0;

  switch (argList.size()) {
    // memdump //
  case 1:
    returnCode = kernel.kernelMemDumpAll();
    break;
  case 2:
    // memdump ../path/to/file //
    if (argList[1] == "-a") {
      kernel.kernelMemDumpEveryAddress();
    }
    returnCode = kernel.kernelMemDump(argList[1]);
    break;
  }
  // too many arguments //
  if (argList.size() > 2) {
    printf("[SH]::mdmp - too many arguments\n");
    return 1;
  }

  // getPcb did not return anything
  if (returnCode == 204) {
    std::cout << "[SH]::mdmp - " << argList[1] << " was not found";
  }
  return 0;
}

int Shell::shellSetScheduler(std::vector<std::string> argList) {
  int quantum = 5;
  int ratio = 3;

  if (argList.size() == 1) {
    switch (kernel.schedulerAlgo) {
    case FCFS:
      std::cout << "[SH]::sets - current set algo [FCFS] " << std::endl;
      break;
    case RR:
      std::cout << "[SH]::sets - current set algo [RR], quantum ["
                << kernel.schedulerQuantum << "]" << std::endl;
      break;
    case MLFQ:
      std::cout << "[SH]::sets - current set algo [MLFQ], quantum ["
                << kernel.schedulerQuantum << "], ratio [" << kernel.mlfqRatio
                << "]" << std::endl;
      break;
    }
    return 1;
  }

  if (argList.size() >= 3) {
    try {
      quantum = stoi(argList[2]);
    } catch (const std::invalid_argument &) {
      std::cout << "[SH]::sets - invalid use" << std::endl;
      return 1;
    }
  }

  if (argList.size() >= 4) {
    try {
      quantum = stoi(argList[2]);
      ratio = stoi(argList[3]);
    } catch (const std::invalid_argument &) {
      std::cout << "[SH]::sets - invalid use" << std::endl;
      return 1;
    }
  }

  int returnCode = kernel.kernelSetScheduler(argList[1], quantum, ratio);
  switch (returnCode) {
  case FAILURE:
    std::cout << "[SH]::sets - invalid use" << std::endl
              << "'sets 'algo' 'quant' 'ratio'" << std::endl;
    return FAILURE;
  case FCFS:
    std::cout << "[SH]::sets - scheduler set to " << "[FCFS]" << std::endl;
    break;
  case RR:
    std::cout << "[SH]::sets - scheduler set to " << "[RR],"
              << " quantum [" << quantum << "]," << std::endl;
    break;
  case MLFQ:
    std::cout << "[SH]::sets - scheduler set to " << "[MLFQ]"
              << ", quantum [" << quantum << "], ratio [" << ratio << "]"
              << std::endl;
    break;
  }
  return SUCCESS;
}

int Shell::shellWriteOut(std::vector<std::string> argList) {
  switch (argList.size()) {
  case 1:
    std::cout << "[SH]::wout - invalid use" << std::endl;
    return 1;
  case 2:
    kernel.kernelWriteOut(argList[1]);
    return 1;
  case 3:
    try {
      kernel.kernelWriteOut(argList[1], stoi(argList[2]));
      return 0;
    } catch (const std::invalid_argument &) {
      std::cout << "[SH]::wout - invalid use" << std::endl;
      return 1;
    }
  }
  return 0;
}

int Shell::shellInitSemaphore(std::vector<std::string> argList) {
  int size = 1;
  if (argList.size() < 4) {
    std::cout
        << "[SH]::insm - invalid use provide: insm 'sem value' 'loaded prcOne' "
           "'loaded prcTwo'"
        << std::endl;
    return 1;
  }
  std::string pTwo = argList[3];
  std::string pOne = argList[2];

  try {
    size = stoi(argList[1]);
  } catch (const std::invalid_argument &) {
    std::cout << "[SH]::oshm - invalid use provide: oshm size, pName, pName"
              << std::endl;
    return 1;
  }

  int returnCode = kernel.kernelInitSemaphore(size, pOne, pTwo);
  if (returnCode == -1) {
    std::cout << "[SH]::oshm - init semaphore failed, check logs\n";
    return 1;
  }

  std::cout << "[SH]::insm - new semaphore [" << returnCode << "]" << std::endl;
  return 0;
}

int Shell::shellOpenSharedMemory(std::vector<std::string> argList) {
  // oshm (5, prod.osx, com.osx)
  // print smid = ??
  int size = 0;

  if (argList.size() < 4) {
    std::cout << "[SH]::oshm - invalid use provide: oshm size, pName, pName"
              << std::endl;
    return 1;
  }
  std::string pOne = argList[3];
  std::string pTwo = argList[2];

  try {
    size = stoi(argList[1]);
  } catch (const std::invalid_argument &) {
    std::cout << "[SH]::oshm - invalid use provide: oshm size, pName, pName"
              << std::endl;
    return 1;
  }

  int returnCode = kernel.kernelOpenSharedMemory(size, pOne, pTwo);

  if (returnCode == -1) {
    std::cout << "[SH]::oshm - open shared memory failed, check logs\n";
    return 1;
  }

  int address = returnCode;

  std::cout << "[SH]::oshm - [" << pOne << "] <--> [" << pTwo << "] addr::["
            << std::hex << address << "] size::[" << size << "]" << std::endl;

  return 0;
}

int Shell::shellTest(std::vector<std::string> argList) {

  if (argList.size() < 2 || argList.size() >= 3) {
    std::cout << "[SH]::test - invalid use" << std::endl;
    return 1;
  }

  // test 1
  switch (stoi(argList[1])) {
  case 1: {
    shellSetScheduler(commandStringToArrayOfStrings("sets mlfq 3 2"));
    shellExecute(commandStringToArrayOfStrings(
        "exec ../demo/0.osx ../demo/1.osx 300 ../demo/bouncing.osx 25"));
    break;
  }
  case 2: {
    shellSetScheduler(commandStringToArrayOfStrings("sets rr 7"));

    shellLoad((commandStringToArrayOfStrings(
        "load  ../prodCom/prod.osx  ../prodCom/com.osx")));

    shellOpenSharedMemory((commandStringToArrayOfStrings(
        "oshm 5  ../prodCom/prod.osx ../prodCom/com.osx")));
    shellInitSemaphore((commandStringToArrayOfStrings(
        "insm 1  ../prodCom/prod.osx ../prodCom/com.osx")));
    break;
  }
  case 3: {

    int countOffset = 0;
    std::cout << "running tests....\n";
    std::string smCpu = "exec";
    for (int i = 0; i < 6; i++) {
      smCpu.append(" ../m3/smCpu/");
      smCpu.append(std::to_string(i));
      smCpu.append(".osx ");
      smCpu.append(std::to_string(countOffset));
      countOffset += 50;
    }
    shellSetScheduler(commandStringToArrayOfStrings("sets mlfq 5 3"));
    shellExecute(commandStringToArrayOfStrings(smCpu));
    break;
  }
  case 4: {
    int countOffset = 0;
    std::cout << "running tests....\n";
    std::string smCpu = "exec";
    for (int i = 0; i < 6; i++) {
      smCpu.append(" ../m3/smCpu/");
      smCpu.append(std::to_string(i));
      smCpu.append(".osx ");
      smCpu.append(std::to_string(countOffset));
      countOffset += 50;
    }
    shellSetScheduler(commandStringToArrayOfStrings("sets rr 7"));
    shellExecute(commandStringToArrayOfStrings(smCpu));
  }
  }
  return 0;
}
