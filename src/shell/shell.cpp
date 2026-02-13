#include "shell.h"
// inits my unordered_map and path to vm

Shell::Shell(Kernel &kernel) : kernel(kernel) {}

void Shell::initPath() {
  functionMap["run"] = &Shell::shellRun;
  functionMap["load"] = &Shell::shellLoadProgram;
  functionMap["exit"] = &Shell::shellExitShell;
  functionMap["help"] = &Shell::shellPrintHelp;
  functionMap["clear"] = &Shell::shellPrintClear;
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
    if (returnCode == 9) { // EXIT CODE
      return 0;
    }
  }
}

//////////////////////////////
// CONSOLE CONTROL PROGRAMS //
//////////////////////////////

int Shell::shellPrintClear(std::vector<std::string> argList) {
  system("clear");
  return 0;
}

int Shell::shellPrintHelp(std::vector<std::string> argList) {
  std::cout
      << "\n[SHELL] --HELP  \n==============\n"
         "\nrun [-v] ------ runs a loaded specified program\n"
         "load [-v]------ loads a binary file from an input file path\n"
         "clear --------- clears the terminal screen\n"
         "clear --------- clears the terminal screen\n"
         "coredump ------ lists the current values contained in REGISTERS\n"
         "errordump ----- prints logged errors\n"
         "exit ---------- exits terminal\n\n";
  return 0;
}

int Shell::shellExitShell(std::vector<std::string> argList) {
  std::cout << "exiting...\n";
  return 9;
}

/////////////////////////////
// MEMORY CONTROL PROGRAMS //
/////////////////////////////

int Shell::shellLoadProgram(std::vector<std::string> argList) {
  int returnCode;

  // load
  if (argList.size() <= 1) {
    std::cout << "[SHELL] --no file path provided--\n";
    return 1;
  }

  // load -v
  if (argList[1] == "-v" && argList.size() == 2) {
    std::cout << "[SHELL] --no file path provided--\n";
    return 1;
  }

  // load -v "path/to/file"
  if (argList[1] == "-v" && argList.size() > 2) {
    returnCode = kernel.loader.loadProgram(argList[2]);
    // replace with memDump(argList[2]);
    kernel.errorHandler.memDumpAll();
    return 0;
  }

  // load "path/to/file"
  returnCode = kernel.loader.loadProgram(argList[1]);

  // loader found a memory overflow
  if (returnCode == 201) {
    std::cout << "[SHELL] -- error 201 returned by kernel\n";
    kernel.errorHandler.errorList.push_back(returnCode);
    return 1;
  }

  // loader found memory occupation
  if (returnCode == 202) {
    std::cout << "[SHELL] -- error [202] returned by kernel\n";
    kernel.errorHandler.errorList.push_back(returnCode);
    return 1;
  }

  return 0;
}

int Shell::shellErrorDump(std::vector<std::string> argList) {
  kernel.errorHandler.errorDump();
  return 0;
}

int Shell::shellRun(std::vector<std::string> argList) {
  int returnCode;

  // run
  if (argList.size() == 1) {
    returnCode = kernel.scheduler.runProgram();

    if (returnCode == 100) {
      std::cout << "[SHELL] --error [100] returned by the kernel\n";
      kernel.errorHandler.errorList.push_back(returnCode);
      return 1;
    }
    return 0;
  }

  // run -v
  if (argList.size() >= 2 && argList[1] == "-v") {

    returnCode = kernel.scheduler.runProgram();

    if (returnCode == 100) {
      std::cout << "[SHELL] --error [100] returned by the kernel\n";
      kernel.errorHandler.errorList.push_back(returnCode);
      return 1;
    }

    kernel.errorHandler.coreDump();
    return 0;
  }

  return 1;
}

int Shell::shellExecute(std::vector<std::string> arglist) {}

int Shell::shellCoreDump(std::vector<std::string> argList) {
  kernel.errorHandler.coreDump();
  return 0;
}

int Shell::shellMemDump(std::vector<std::string> argList) {
  kernel.errorHandler.memDumpAll();
  return 0;
}
