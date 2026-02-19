#ifndef SHELL_H
#define SHELL_H

#include "../kernel/kernel.h"
#include <cstdlib>
#include <hashtable.h>
#include <iostream>
#include <string>

class Shell {
public:
  Shell(Kernel &kernel);
  Kernel &kernel;

  std::unordered_map<std::string, int (Shell::*)(std::vector<std::string>)>
      functionMap;

  // inits my unordered_map and path to vm
  void initPath();

  std::vector<std::string>
  commandStringToArrayOfStrings(std::string commandString);

  ///////////////
  // MAIN LOOP //
  ///////////////

  int shellLoop();

  //////////////////////////////
  // CONSOLE CONTROL PROGRAMS //
  //////////////////////////////

  int shellClear(std::vector<std::string> argList);

  int shellHelp(std::vector<std::string> argList);

  /////////////////////////////
  // MEMORY CONTROL PROGRAMS //
  /////////////////////////////

  int shellLoad(std::vector<std::string> argList);

  int shellErrorDump(std::vector<std::string> argList);

  int shellMemDump(std::vector<std::string> argList);

  /////////////////////////////////////
  // VIRTUAL MACHINE CONTROL PROGRAMS//
  /////////////////////////////////////

  int shellRun(std::vector<std::string> argList);

  int shellExecute(std::vector<std::string> argList);

  int shellCoreDump(std::vector<std::string> argList);

  int shellExit(std::vector<std::string> argList);
};

#endif
