#ifndef SHELL_H
#define SHELL_H

#include "kernel.h"
#include <cstdlib>
#include <hashtable.h>
#include <iostream>
#include <string>

class Shell {
public:
  Kernel MainKernel;

  std::unordered_map<std::string, int (Shell::*)(std::vector<std::string>)>
      functionMap;

  // inits my unordered_map and path to vm
  void initPath() {
    functionMap["run"] = &Shell::shellRunProgram;
    functionMap["load"] = &Shell::shellLoadProgram;
    functionMap["exit"] = &Shell::shellExitShell;
    functionMap["help"] = &Shell::shellPrintHelp;
    functionMap["clear"] = &Shell::shellPrintClear;
    functionMap["coredump"] = &Shell::shellCoreDump;
    functionMap["errordump"] = &Shell::shellErrorDump;
  }

  //////////////////////////////////////
  // INPUT ARGUMENT TO ARGUMENT ARRAY //
  //////////////////////////////////////

  std::vector<std::string>
  commandStringToArrayOfStrings(std::string commandString) {
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

  int shellLoop() {

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

  int shellPrintClear(std::vector<std::string> argList) {
    system("clear");
    return 0;
  }

  int shellPrintHelp(std::vector<std::string> argList) {
    std::cout
        << "\nHELP  \n========\n"
           "\nrun [-v] ------ runs a loaded specified program\n"
           "load [-v]------ loads a binary file from an input file path\n"
           "clear --------- clears the terminal screen\n"
           "clear --------- clears the terminal screen\n"
           "coredump ------ lists the current values contained in REGISTERS\n"
           "errordump ----- prints logged errors\n"
           "exit ---------- exits terminal\n";
    return 0;
  }

  int shellExitShell(std::vector<std::string> argList) {
    std::cout << "EXITING...\n";
    return 9;
  }

  /////////////////////////////
  // MEMORY CONTROL PROGRAMS //
  /////////////////////////////

  int shellLoadProgram(std::vector<std::string> argList) {
    u_int8_t returnCode;
    if (argList.size() <= 1) {
      std::cout << "[SHELL] --no file path provided--\n";
      return 1;
    }

    if (argList[1] == "-v" && argList.size() == 2) {
      std::cout << "[SHELL] --no file path provided--\n";
      return 1;
    }

    if (argList[1] == "-v" && argList.size() > 2) {
      returnCode = MainKernel.loadProgram(argList[2]);
      MainKernel.memDump();
      return 0;
    }

    returnCode = MainKernel.loadProgram(argList[1]);

    if (returnCode == 201) {
      std::cout << "[SHELL] -- error 201 returned by kernel\n";
      return 1;
    }

    if (returnCode == 202) {
      std::cout << "[SHELL] -- error 202 returned by kernel\n";
      return 1;
    }

    return 0;
  }

  int shellErrorDump(std::vector<std::string> argList) {
    MainKernel.errorDump();
    return 0;
  }

  /////////////////////////////////////
  // VIRTUAL MACHINE CONTROL PROGRAMS//
  /////////////////////////////////////

  int shellRunProgram(std::vector<std::string> argList) {

    if (argList.size() == 1) {

      if (MainKernel.runProgram() == 100) {
        std::cout << "KERNEL_ERROR::[100]\n";
        return 1;
      }
      return 0;
    }

    if (argList.size() >= 2 && argList[1] == "-v") {

      if (MainKernel.runProgram() == 100) {
        std::cout << "KERNEL_ERROR::[100]";
        return 1;
      }

      MainKernel.coreDump();
      return 0;
    }
    return 1;
  }

  int shellCoreDump(std::vector<std::string> argList) {
    MainKernel.coreDump();
    MainKernel.memDump();
    return 0;
  }
};

#endif
