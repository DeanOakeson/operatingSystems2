#ifndef ERROR_H
#define ERROR_H

#include "../hardware/vm.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdio.h>
#include <stdlib.h>

static const u_int32_t CPU_EARLY_TERMINATION = 100;
static const u_int32_t LOAD_FILE_NOT_FOUND = 200;
static const u_int32_t MEM_OVERFLOW = 201;
static const u_int32_t MEM_OVERWRITE = 202;
static const u_int32_t MEM_DUMP_NO_PROGRAM = 301;
static const u_int32_t MEM_DUMP_FALSE_PROGRAM = 302;

class ErrorHandler {
public:
  VirtualMachine &machine;
  std::string filePath = "";
  std::vector<int> errorList = {};

  ErrorHandler(VirtualMachine &machine);
  void errorDump();
  void coreDump();
  int memDumpAll();
  int memDump(std::string filePath);
};
#endif
