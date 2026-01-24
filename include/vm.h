#ifndef VM_H
#define VM_H

#include "ram.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdio.h>
#include <stdlib.h>

class VirtualMachine {
public:
  // ARTIHMATIC
  static const u_int8_t ADD = 16;
  static const u_int8_t SUB = 17;
  static const u_int8_t MUL = 18;
  static const u_int8_t DIV = 19;

  // MOVE DATA
  static const u_int8_t MOV = 1;
  static const u_int8_t MVI = 22;
  static const u_int8_t ADR = 0;
  static const u_int8_t STR = 2;
  static const u_int8_t STRB = 3;
  static const u_int8_t LDR = 4;
  static const u_int8_t LDRB = 5;

  // BRANCH
  static const u_int8_t B = 7;
  static const u_int8_t BL = 21;
  static const u_int8_t BX = 6;
  static const u_int8_t BNE = 8;
  static const u_int8_t BGT = 9;
  static const u_int8_t BLT = 10;
  static const u_int8_t BEQ = 11;

  // LOGICAL
  static const u_int8_t CMP = 12;
  static const u_int8_t AND = 13;
  static const u_int8_t ORR = 14;
  static const u_int8_t EOR = 15;
  static const u_int8_t SWI = 20;

  // REGISTERS//
  int reg[6];
  u_int32_t pc;
  int z;

  // Main Memory
  Ram MainMemory;

  void idleCPU();
  void runProgram() {

    // GET RID OF THIS EVENTUALLY//
    int fileSize = MainMemory.fileSize;
    int fileFirstInstruction = MainMemory.fileFirstInstruction;
    int fileLoadedAddress = MainMemory.fileLoadAddress;
    u_int8_t opCode;

    for (pc = fileFirstInstruction; pc < fileSize + fileLoadedAddress; pc++) {
      opCode = MainMemory.mem[pc][0];
      switch (opCode) {

        // ARITHMATIC //
      case ADD: // 16 // WORKS
        reg[MainMemory.mem[pc][1]] =
            reg[MainMemory.mem[pc][2]] + reg[MainMemory.mem[pc][3]];
        break;

      case SUB: // 17 // WORKS
        reg[MainMemory.mem[pc][1]] =
            reg[MainMemory.mem[pc][2]] - reg[MainMemory.mem[pc][3]];
        break;

      case MUL: { // 18 // WORKS
        reg[MainMemory.mem[pc][1]] =
            reg[MainMemory.mem[pc][2]] * reg[MainMemory.mem[pc][3]];
        break;
      }

      case DIV: // 19
        reg[MainMemory.mem[pc][1]] =
            reg[MainMemory.mem[pc][2]] / reg[MainMemory.mem[pc][3]];
        break;

        // MOVE DATA //
      case MOV: // WORKS
        reg[MainMemory.mem[pc][1]] = reg[MainMemory.mem[pc][2]];
        break;

      case MVI:
        reg[MainMemory.mem[pc][1]] = MainMemory.mem[pc][2];
        break;

      case ADR: // TEST // MAYBE IS DONE??? CHECK???
        reg[MainMemory.mem[pc][0]] =
            (MainMemory.mem[pc][1] << 24 | MainMemory.mem[pc][2] << 16 |
             MainMemory.mem[pc][3] << 8 | MainMemory.mem[pc][4]);

        break;
      case STR:
        break;
      case STRB:
        break;
      case LDR:
        break;
      case LDRB:
        break;

        // BREAK //
      case B: // TEST
        // bit shift to combine all uchars into an int
        pc = (MainMemory.mem[pc][1] << 24 | MainMemory.mem[pc][2] << 16 |
              MainMemory.mem[pc][3] << 8 | MainMemory.mem[pc][4]);
        break;
      case BL:
        break;
      case BX: // JUMP TO ADDRESS IN REG, pc <-- <reg>
        pc = (MainMemory.mem[pc][1]);
        break;
      case BNE:
        break;
      case BGT:
        break;
      case BLT:
        break;
      case BEQ:
        break;

        // LOGICAL //
      case CMP:
        break;
      case AND:
        break;
      case ORR:
        break;
      case EOR:
        break;
      case SWI: // swi 10 = halt
        break;
      }
    }
  }

  void coreDump() {
    u_int8_t regId = 0;

    for (u_int8_t i = 0; i < 6; i++) {
      printf("\n-- reg::%d -- %d", regId, reg[regId]);
      regId += 1;
    }

    printf("\n-- reg::pc - %d\n-- reg::z -- %d\n\n", pc, z);
  }
};

#endif
