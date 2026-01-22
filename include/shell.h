#ifndef SHELL_H
#define SHELL_H

#include "ram.h"
#include "vm.h"
#include <fstream>
#include <hashtable.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class Shell {
public:
  VirtualMachine Machine;
  Ram MainMemory;

  void initShell(VirtualMachine NewMachine, Ram NewMainMemory) {
    Machine = NewMachine;
    MainMemory = NewMainMemory;
  }

  void load(std::string filePath) {
    printf("load");
    MainMemory.loadProgram(filePath, &MainMemory);
  }

  void run() { Machine.runProgram(&MainMemory); }

  void clear() {
    for (int i = 0; i < 100; i++) {
      printf("\n");
    }
  }

  void coredump() { Machine.coreDump(); }

  void help() {
    printf("\n--HELP--\n");
    printf("\nrun [-v] ------ runs a loaded specified program\n");
    printf("load [-v]------ loads a binary file from an input file path\n");
    printf("clear --------- clears the terminal screen\n");
    printf("coredump ------ lists the current values contained in REGISTERS\n");
    printf("errordump ----- IDK YET???\n");
    printf("exit ---------- exits terminal\n");
  }

  void errordump() {
    printf("\n\nERRORDUMP\n");
    printf("%d ", MainMemory.mem[12][0]);
    printf("%d ", MainMemory.mem[12][1]);
    printf("%d ", MainMemory.mem[12][2]);
    printf("%d ", MainMemory.mem[12][3]);
    printf("%d ", MainMemory.mem[12][4]);
    printf("%d\n", MainMemory.mem[12][5]);

    printf("%d ", MainMemory.mem[13][0]);
    printf("%d ", MainMemory.mem[13][1]);
    printf("%d ", MainMemory.mem[13][2]);
    printf("%d ", MainMemory.mem[13][3]);
    printf("%d ", MainMemory.mem[13][4]);
    printf("%d\n", MainMemory.mem[13][5]);

    printf("%d ", MainMemory.mem[14][0]);
    printf("%d ", MainMemory.mem[14][1]);
    printf("%d ", MainMemory.mem[14][2]);
    printf("%d ", MainMemory.mem[14][3]);
    printf("%d ", MainMemory.mem[14][4]);
    printf("%d\n", MainMemory.mem[14][5]);

    printf("%d ", MainMemory.mem[15][0]);
    printf("%d ", MainMemory.mem[15][1]);
    printf("%d ", MainMemory.mem[15][2]);
    printf("%d ", MainMemory.mem[15][3]);
    printf("%d ", MainMemory.mem[15][4]);
    printf("%d\n", MainMemory.mem[15][5]);

    printf("%d ", MainMemory.mem[16][0]);
    printf("%d ", MainMemory.mem[16][1]);
    printf("%d ", MainMemory.mem[16][2]);
    printf("%d ", MainMemory.mem[16][3]);
    printf("%d ", MainMemory.mem[16][4]);
    printf("%d\n", MainMemory.mem[16][5]);

    printf("%d ", MainMemory.mem[17][0]);
    printf("%d ", MainMemory.mem[17][1]);
    printf("%d ", MainMemory.mem[17][2]);
    printf("%d ", MainMemory.mem[17][3]);
    printf("%d ", MainMemory.mem[17][4]);
    printf("%d\n", MainMemory.mem[17][5]);
  };

  int shellLoop() {
    std::cout << "SHELL-->> ";
    std::string input;
    std::string command;
    std::string arg;
    while (true) {
      std::cin >> command;

      if (command == "coredump") {
        coredump();
        shellLoop();
        return 0;
      }

      if (command == "run") {
        run();
        shellLoop();
        return 0;
      }

      if (command == "load") {
        load(arg);
        shellLoop();
        return 0;
      }

      if (command == "exit") {
        printf("EXITING SHELL..\n");
        return 0;
      }

      if (command == "errordump") {
        errordump();
        shellLoop();
        return 0;
      }

      if (command == "clear") {
        clear();
        shellLoop();
        return 0;
      }

      if (command == "help") {
        help();
        shellLoop();
        return 0;
      }

      else {
        printf("\ntype help for all available commands\n\n");
        shellLoop();
        return 0;
      }
    }
  }
};

#endif
