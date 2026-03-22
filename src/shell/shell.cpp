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

  return 0;
}

void Shell::initPath() {
  functionMap["run"] = &Shell::shellRun;
  functionMap["load"] = &Shell::shellLoad;
  functionMap["exit"] = &Shell::shellExit;
  functionMap["help"] = &Shell::shellHelp;
  functionMap["clear"] = &Shell::shellClear;
  functionMap["coredump"] = &Shell::shellCoreDump;
  functionMap["errordump"] = &Shell::shellErrorDump;
  functionMap["memdump"] = &Shell::shellMemDump;
  functionMap["exec"] = &Shell::shellExecute;
  functionMap["gannt"] = &Shell::shellGannt;
  functionMap["-v"] = &Shell::shellVerbose;
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
    printf("[SH]::load -- no file provided\n");
    return 1;
  case 2:
    // load ...
    returnCode = kernel.kernelLoadProgram(argList[1]);
    if (returnCode == 1) {
      printf("[SH]::load -- attemted load failed\n");
    }
    return 0;
  case 3:
    // load -v ...
    returnCode = kernel.kernelLoadProgram(argList[2]);
    if (returnCode == 1) {
      printf("[SH]::load -- attemted load failed\n");
    }
    kernel.kernelMemDump(argList[2]);
    return 0;
  }
  printf("[SH]::load -- too many arguments\n");
  return 1;
}

int Shell::shellErrorDump(std::vector<std::string> argList) {
  int returnCode;
  switch (argList.size()) {
  case 1:
    returnCode = kernel.kernelErrorDump();
    if (returnCode == 1) {
      printf("[SH]::edmp -- no logged errors\n");
    }
    return 0;
  }
  printf("[SH]::load -- too many arguments\n");
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
  printf("[SH]::run -- not valid usage\n");
  return 1;
}

int Shell::shellExecute(std::vector<std::string> argList) {
  std::multimap<int, std::string> argMap;
  std::multimap<int, std::string> returnMap;

  // argList[0] is the command name ("exec"), so start at index 1
  // Format: <program> [arrivalTime] <program> [arrivalTime] ...
  // If no arrival time follows a program, default to 0

  if (argList.size() == 1) {
    printf("[SH]::exec -- no file provided\n");
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
      } catch (std::invalid_argument &) {
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

  switch (argList.size()) {
    // memdump //
  case 1:
    kernel.kernelMemDumpAll();
    return 0;
  case 2:
    // memdump ../path/to/file //
    kernel.kernelMemDump(argList[1]);
    return 0;
  }
  // too many arguments //
  if (argList.size() > 2) {
    printf("[SH]::mdmp - too many arguments\n");
    return 1;
  }
  return 0;
}
