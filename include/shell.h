#ifndef SHELL_H
#define SHELL_H

#include "ram.h"
#include "vm.h"
#include <hashtable.h>
#include <iostream>
#include <string>

// create asm to loop thru 1 to 100
// kernel mode for SWI
// document
// watch the other lecture i feel i am missing something others are not.
// location.`` ask tim how he implemented the kernel?? finish implementing my

class Shell {
public:
  VirtualMachine Machine;

  std::unordered_map<std::string, int (Shell::*)(std::vector<std::string>)>
      functionMap;

  // inits my unordered_map and path to vm
  void initPath(VirtualMachine NewMachine) {
    Machine = NewMachine;
    functionMap["run"] = &Shell::runProgram;
    functionMap["load"] = &Shell::loadProgram;
    functionMap["exit"] = &Shell::exitShell;
    functionMap["help"] = &Shell::printHelp;
    functionMap["clear"] = &Shell::printClear;
    functionMap["coredump"] = &Shell::coreDump;
    functionMap["errordump"] = &Shell::errorDump;
  }

  // takes command input string and converts it to an array of strings returns
  // array
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

  // main loop returns an int 1 for failure 0 for success

  int shellLoop() {

    while (true) {
      printf("SHELL-->> ");
      std::string command;
      std::getline(std::cin, command);

      std::vector<std::string> commandArray =
          commandStringToArrayOfStrings(command);

      if (functionMap.find(commandArray[0]) != functionMap.end()) {
        int returnCode = (this->*functionMap[commandArray[0]])(commandArray);
        if (returnCode == 9) { // EXIT CODE
          return 0;
        }
      } else { // IF FUNCTION IS NOT FOUND IN THE LIST PRINT AND RUN LOOP
        printf("Command not found. Try 'help'\n");
      }
    }
  }

  int loadProgram(std::vector<std::string> argList) {
    if (argList.size() <= 1) {
      printf("ERROR --NO FILE PATH PROVIDED--\n");
      return 1;
    } else {
      u_int8_t returnCode = Machine.MainMemory.loadProgram(argList[1]);
      if (returnCode == 1) {
        printf("ERROR --MEMORY IS OCCUPIED--\n");
        return 1;
      }
      if (returnCode == 2) {
        printf("ERROR --MEMORY OVERFLOW--\n");
        return 1;
      }
    }
    return 0;
  }

  // COMMAND PROGRAMS//

  int runProgram(std::vector<std::string> argList) {
    Machine.runProgram();
    return 0;
  }

  int printClear(std::vector<std::string> argList) {
    for (int i = 0; i < 100; i++) {
      printf("\n");
    }
    return 0;
  }

  int coreDump(std::vector<std::string> argList) {
    Machine.coreDump();
    return 0;
  }

  int printHelp(std::vector<std::string> argList) {
    printf("\n--HELP--\n"
           "\nrun [-v] ------ runs a loaded specified program\n"
           "load [-v]------ loads a binary file from an input file path\n"
           "clear --------- clears the terminal screen\n"
           "clear --------- clears the terminal screen\n"
           "coredump ------ lists the current values contained in REGISTERS\n"
           "errordump ----- IDK YET???\n"
           "exit ---------- exits terminal\n");
    return 0;
  }

  int errorDump(std::vector<std::string> argList) {
    Machine.MainMemory.memDump();
    return 0;
  }

  int exitShell(std::vector<std::string> argList) {
    printf("EXITING...\n");
    return 9;
  }
};

#endif
