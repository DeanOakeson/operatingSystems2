#ifndef VM_H
#define VM_H

#include "ram.h"
#include <bits/stdc++.h>
#include <hashtable.h>
#include <stdio.h>
#include <stdlib.h>

class VirtualMachine {
public:
  ///////////////////
  // STATIC CONSTS //
  ///////////////////

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
  int PC = 0;
  int SP = 0;
  int FP = 0;
  int SL = 0;
  int Z = 0;
  int SB = 0;
  int byteOffset = 0;
  int swiOpCode = 0;

  Ram MainMemory;

  ////////////////////
  // CPU OPERATIONS //
  ////////////////////

  // 100 = CPU TERMINATION CODE
  int runCpu() {
    int userMode = 0;
    int kernelMode = 0;
    PC = MainMemory.fileLoadAddress;

    while (PC < MainMemory.fileSize + MainMemory.fileLoadAddress) {
      uint8_t opcode = MainMemory.mem[PC][0];
      bool incrementPC = true;

      switch (opcode) {
      // ARITHMATIC //
      case ADD: // ADDS reg<1> <-- reg<2> + reg<3>
        reg[MainMemory.mem[PC][1]] =
            reg[MainMemory.mem[PC][2]] + reg[MainMemory.mem[PC][3]];
        break;
      case SUB: // SUB reg<1> <-- reg<2> - reg<3>
        reg[MainMemory.mem[PC][1]] =
            reg[MainMemory.mem[PC][2]] - reg[MainMemory.mem[PC][3]];
        break;
      case MUL: { // MUL reg<1> <-- reg<2> * reg<3>
        reg[MainMemory.mem[PC][1]] =
            reg[MainMemory.mem[PC][2]] * reg[MainMemory.mem[PC][3]];
        break;
      }
      case DIV: // DIV reg<1> <-- reg<2> / reg<3>
        reg[MainMemory.mem[PC][1]] =
            reg[MainMemory.mem[PC][2]] / reg[MainMemory.mem[PC][3]];
        break;
      case MOV: // MOVE reg<1> <-- reg<2>
        reg[MainMemory.mem[PC][1]] = reg[MainMemory.mem[PC][2]];
        break;
      case MVI: // MOVE IMM reg<1> <-- reg<2> | reg<3> |
        reg[MainMemory.mem[PC][1]] =
            (int32_t)((uint32_t)MainMemory.mem[PC][5] << 24 |
                      (uint32_t)MainMemory.mem[PC][4] << 16 |
                      (uint32_t)MainMemory.mem[PC][3] << 8 |
                      (uint32_t)MainMemory.mem[PC][2]);
        break;
      case ADR:
        break;
      case STR:
        break;
      case STRB:
        break;
      case LDR:
        break;
      case LDRB:
        break;
      // BRANCH //
      case B:
        byteOffset = (int32_t)(((uint32_t)MainMemory.mem[PC][4] << 24 |
                                (uint32_t)MainMemory.mem[PC][3] << 16 |
                                (uint32_t)MainMemory.mem[PC][2] << 8 |
                                (uint32_t)MainMemory.mem[PC][1]));
        // TRANSLATE INTO 6 BYTE ADDRESS SPACE
        PC = byteOffset / 6 + MainMemory.fileLoadAddress;
        incrementPC = false;
        break;
      case BL:
        break;
        PC = reg[MainMemory.mem[PC][1]];
        incrementPC = false;
        break;
      case BNE:
        byteOffset = (int32_t)(((uint32_t)MainMemory.mem[PC][4] << 24 |
                                (uint32_t)MainMemory.mem[PC][3] << 16 |
                                (uint32_t)MainMemory.mem[PC][2] << 8 |
                                (uint32_t)MainMemory.mem[PC][1]));
        if (Z == 1) {
          PC = byteOffset / 6 + MainMemory.fileLoadAddress;
          this->Z = 0;
          incrementPC = false;
        }
        break;
      case BGT:
        break;
      case BLT:
        break;
      case BEQ:
        break;
      // LOGICAL //
      case CMP: // CMP <reg1> - <reg2>
        if (reg[MainMemory.mem[PC][1]] == reg[MainMemory.mem[PC][2]]) {
          Z = 1;
          break;
        }
        break;
      case AND:
        break;
      case ORR:
        break;
      case EOR:
        break;
      case SWI: // SWI 10 = HALT
        swiOpCode = (int32_t)(((uint32_t)MainMemory.mem[PC][4] << 24 |
                               (uint32_t)MainMemory.mem[PC][3] << 16 |
                               (uint32_t)MainMemory.mem[PC][2] << 8 |
                               (uint32_t)MainMemory.mem[PC][1]));
        if (kernelMode == 0) {
          std::string userInput;

          while (true) {
            std::cin.clear();
            std::cout << "[OS] -- allow kernel access?, press y/n \n";
            std::cin >> userInput;
            std::cin.ignore();

            if (userInput == "n" | userInput == "N") {
              std::cout << "[OS] -- program not permitted to enter kernel "
                           "mode. program "
                           "terminated.\n";
              std::cin.clear();
              return 100;
            }
            if (userInput == "y" | userInput == "Y") {
              kernelMode = 1;
              std::cin.clear();
              std::cout << "[OS] -- kernel mode entered\n";
              break;
            }
            std::cout << "[OS] -- invalid input\n";
          }
        }

        switch (swiOpCode) {
        case 1:
          std::cout << "[OS][SWI 1] -- not yet defined\n";
          break;
        case 2:
          break;
        case 10: // HALT SUCCESFULLY
          std::cout << "[OS][SWI 10] -- halting program--\n";
          return 0;
        }
        break;
      }

      // Only increment PC if we didn't branch
      if (incrementPC) {
        PC++;
      }
    }
    kernelMode = 0;
    return 0;
  }
};

#endif
