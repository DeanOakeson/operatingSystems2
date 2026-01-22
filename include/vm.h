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

  void idleCPU();
  void runProgram(Ram *pMainMemory) {

    // GET RID OF THIS EVENTUALLY//
    int fileSize = 36;
    int fileFirstInstruction = 12;
    int fileLoadedAddress = 0;
    u_int8_t opCode;

    for (pc = fileFirstInstruction; pc < fileSize + fileLoadedAddress; pc++) {
      opCode = pMainMemory->mem[pc][0];
      switch (opCode) {

        // ARITHMATIC //
      case ADD: // 16 // WORKS
        reg[pMainMemory->mem[pc][1]] =
            reg[pMainMemory->mem[pc][2]] + reg[pMainMemory->mem[pc][3]];
        break;

      case SUB: // 17 // WORKS
        reg[pMainMemory->mem[pc][1]] =
            reg[pMainMemory->mem[pc][2]] - reg[pMainMemory->mem[pc][3]];
        break;

      case MUL: { // 18 // WORKS
        reg[pMainMemory->mem[pc][1]] =
            reg[pMainMemory->mem[pc][2]] * reg[pMainMemory->mem[pc][3]];
        break;
      }

      case DIV: // 19
        reg[pMainMemory->mem[pc][1]] =
            reg[pMainMemory->mem[pc][2]] / reg[pMainMemory->mem[pc][3]];
        break;

        // MOVE DATA //
      case MOV: // WORKS
        reg[pMainMemory->mem[pc][1]] = reg[pMainMemory->mem[pc][2]];
        break;

      case MVI:
        reg[pMainMemory->mem[pc][1]] = pMainMemory->mem[pc][2];
        break;

      case ADR: // TEST // MAYBE IS DONE??? CHECK???
        reg[pMainMemory->mem[pc][0]] =
            (pMainMemory->mem[pc][1] << 24 | pMainMemory->mem[pc][2] << 16 |
             pMainMemory->mem[pc][3] << 8 | pMainMemory->mem[pc][4]);

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
        pc = (pMainMemory->mem[pc][1] << 24 | pMainMemory->mem[pc][2] << 16 |
              pMainMemory->mem[pc][3] << 8 | pMainMemory->mem[pc][4]);
        break;
      case BL:
        break;
      case BX:
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
      printf("\n");

      printf("[Debug] REG_%d:", regId);
      printf("%d  ", reg[regId]);
      regId += 1;
    }

    printf("\n[Debug] PC:%d\n", pc);
    printf("[Debug] Z:%d\n\n", z);
  }
};

#endif
