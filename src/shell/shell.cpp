#include "shell.h"
// inits my unordered_map and path to vm

Shell::Shell(Kernel &kernel) : kernel(kernel) {}

void Shell::initPath() {
  functionMap["run"] = &Shell::shellRun;
  functionMap["load"] = &Shell::shellLoad;
  functionMap["exit"] = &Shell::shellExit;
  functionMap["help"] = &Shell::shellHelp;
  functionMap["clear"] = &Shell::shellClear;
  functionMap["coredump"] = &Shell::shellCoreDump;
  functionMap["errordump"] = &Shell::shellErrorDump;
  functionMap["memdump"] = &Shell::shellMemDump;
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
    std::cout << "[SHELL]-->> ";
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
    printf("\n");
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
      << "\n[SHELL] --HELP  \n==============\n"
         "\nrun [-v] ------ runs a loaded specified program\n"
         "load [-v]------ loads a binary file from an input file path\n"
         "clear --------- clears the terminal screen\n"
         "clear --------- clears the terminal screen\n"
         "coredump ------ lists the current values contained in REGISTERS\n"
         "errordump ----- prints logged errors\n"
         "exit ---------- exits terminal\n";
  return 0;
}

int Shell::shellExit(std::vector<std::string> argList) {
  std::cout << "[SHELL] exiting...\n";
  return 9;
}

/////////////////////////////
// MEMORY CONTROL PROGRAMS //
/////////////////////////////

int Shell::shellLoad(std::vector<std::string> argList) {

  switch (argList.size()) {
  case 1:
    // error message
  case 2:
    // load ...
    kernel.kernelLoadProgram(argList[1]);
    return 0;
  case 3:
    // load -v ...
    kernel.kernelLoadProgram(argList[2]);
    kernel.kernelMemDump(argList[2]);
    return 0;
  }
  printf("[SHELL] -- too many arguments\n");
  return 1;
}

int Shell::shellErrorDump(std::vector<std::string> argList) {
  switch (argList.size()) {
  case 1:
    kernel.kernelErrorDump();
    return 0;
  }
  printf("[SHELL] -- too many arguments\n");
  return 1;
}

int Shell::shellRun(std::vector<std::string> argList) {

  switch (argList.size()) {
  case 1:
    kernel.kernelRun();
    return 0;

  case 2:
    if (argList[1] == "-v") {
      kernel.kernelRun();
      kernel.kernelCoreDump();
      return 0;
    }
    printf("[SHELL] %s is not a valid argument\n", argList[1].c_str());

    return 1;
  case 3:
    // run -v ../asm/file
    kernel.kernelRunSingleProgram(argList[2]);
    kernel.kernelCoreDump();
    return 0;
  }
  printf("[SHELL] -- too many arguments\n");
  return 1;
}

int Shell::shellExecute(std::vector<std::string> arglist) {}

/////////////////////
// ERROR MANAGMENT //
/////////////////////

int Shell::shellCoreDump(std::vector<std::string> argList) {
  kernel.kernelCoreDump();
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
    printf("[SHELL] too many arguments\n");
    return 1;
  }
  return 0;
}
