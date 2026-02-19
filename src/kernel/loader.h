#ifndef LOADER_H
#define LOADER_H

#include "../hardware/vm.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdio.h>
#include <stdlib.h>

class Loader {
public:
  VirtualMachine &machine;
  static const u_int32_t CPU_EARLY_TERMINATION = 100;
  static const u_int32_t FILE_NOT_FOUND = 200;

  static const u_int32_t MEM_OVERFLOW = 201;
  static const u_int32_t MEM_OVERWRITE = 202;
  static const u_int32_t MEM_FSTREAM_ERR_1 = 203;
  static const u_int32_t MEM_FSTREAM_ERR_2 = 204;
  static const u_int32_t MEM_FSTREAM_ERR_3 = 205;
  static const u_int32_t MEM_DUMP_NO_PROGRAM = 301;

  Loader(VirtualMachine &machine);

  std::string filePath;
  std::vector<int> asmHeader;
  std::tuple<int, std::vector<int>> returnVector;
  std::tuple<int, std::vector<int>> loadProgram(std::string FilePath);

private:
  bool verifyMemoryIsUnoccupied(std::vector<int> asmHeader);
  void updateMemoryIndicators(std::vector<int> asmHeader);
};
#endif
