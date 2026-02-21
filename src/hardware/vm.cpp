#include "vm.h"

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

////////////////////
// CPU OPERATIONS //
////////////////////

// 100 = CPU TERMINATION CODE
int VirtualMachine::runCpu(Pcb &process) {
  int opcode;
  printf("[VM] PC -- %d\n", PC);

  // CPU PREPERATION//
  bool incrementPC = true;

  while (PC <= process.pSize + process.pLoadAddress) {
    opcode = ram.mem[PC][0];
    incrementPC = true;

    switch (opcode) {
    // ARITHMATIC //
    case ADD: // ADDS Reg<1> <-- Reg<2> + Reg<3>
      // printf("[VM] ADDING -- R[%d]\n", ram.mem[PC + 1][0]);
      Reg[ram.mem[PC + 1][0]] =
          Reg[ram.mem[PC + 2][0]] + Reg[ram.mem[PC + 3][0]];
      break;
    case SUB: // SUB Reg<1> <-- Reg<2> - Reg<3>
      // printf("[VM] SUBTRACTING -- R[%d]\n", ram.mem[PC + 1][0]);
      Reg[ram.mem[PC + 1][0]] =
          Reg[ram.mem[PC + 2][0]] + Reg[ram.mem[PC + 3][0]];
      break;
    case MUL: // MUL Reg<1> <-- Reg<2> * Reg<3>
      // printf("[VM] MULTIPLYING -- R[%d]\n", ram.mem[PC + 1][0]);
      Reg[ram.mem[PC + 1][0]] =
          Reg[ram.mem[PC + 2][0]] * Reg[ram.mem[PC + 3][0]];
      break;
    case DIV: // DIV Reg<1> <-- Reg<2> / Reg<3>
      // printf("[VM] DIVIDING -- R[%d]\n", ram.mem[PC + 1][0]);
      Reg[ram.mem[PC + 1][0]] =
          Reg[ram.mem[PC + 2][0]] / Reg[ram.mem[PC + 3][0]];
      break;
    case MOV: // MOVE Reg<1> <-- Reg<2>
      Reg[ram.mem[PC + 1][0]] = Reg[ram.mem[PC + 2][0]];
      break;
    case MVI: // MOVE IMM Reg<1> <-- Reg<2> | Reg<3> |
      printf("[VM] MVI -- R[%d]\n", ram.mem[PC + 1][0]);
      Reg[ram.mem[PC + 1][0]] = (int32_t)((uint32_t)ram.mem[PC + 5][0] << 24 |
                                          (uint32_t)ram.mem[PC + 4][0] << 16 |
                                          (uint32_t)ram.mem[PC + 3][0] << 8 |
                                          (uint32_t)ram.mem[PC + 2][0]);
      break;
    case ADR:
      Reg[ram.mem[PC + 1][0]] = (uint32_t)ram.mem[wordOffset][4] << 24 |
                                (uint32_t)ram.mem[wordOffset][3] << 16 |
                                (uint32_t)ram.mem[wordOffset][2] << 8 |
                                (uint32_t)ram.mem[wordOffset][1];

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
    case B: // BRANCH
      // printf("B\n");
      byteOffset = (int32_t)(((uint32_t)ram.mem[PC + 4][0] << 24 |
                              (uint32_t)ram.mem[PC + 3][0] << 16 |
                              (uint32_t)ram.mem[PC + 2][0] << 8 |
                              (uint32_t)ram.mem[PC + 1][0]));
      PC = byteOffset + process.pLoadAddress;
      incrementPC = false;
      break;

    case BL: // TEST //WRONGG
      byteOffset = (int32_t)(((uint32_t)ram.mem[PC + 4][0] << 24 |
                              (uint32_t)ram.mem[PC + 3][0] << 16 |
                              (uint32_t)ram.mem[PC + 2][0] << 8 |
                              (uint32_t)ram.mem[PC + 1][0]));
      PC = byteOffset + process.pLoadAddress;
      incrementPC = false;
      break;

    case BNE: // BRANCH IF NOT EQUAL works
      byteOffset = (int32_t)(((uint32_t)ram.mem[PC + 4][0] << 24 |
                              (uint32_t)ram.mem[PC + 3][0] << 16 |
                              (uint32_t)ram.mem[PC + 2][0] << 8 |
                              (uint32_t)ram.mem[PC + 1][0]));
      if (Z != 0) {
        PC = byteOffset + process.pLoadAddress;
        incrementPC = false;
      }
      break;

    case BGT: // TEST
      byteOffset = (int32_t)(((uint32_t)ram.mem[PC + 4][0] << 24 |
                              (uint32_t)ram.mem[PC + 3][0] << 16 |
                              (uint32_t)ram.mem[PC + 2][0] << 8 |
                              (uint32_t)ram.mem[PC + 1][0]));
      if (Z > 0) {
        PC = byteOffset + process.pLoadAddress;
        incrementPC = false;
      }
      break;

    case BLT: // TEST
      byteOffset = (int32_t)(((uint32_t)ram.mem[PC + 4][0] << 24 |
                              (uint32_t)ram.mem[PC + 3][0] << 16 |
                              (uint32_t)ram.mem[PC + 2][0] << 8 |
                              (uint32_t)ram.mem[PC + 1][0]));

      if (Z < 0) {
        PC = byteOffset + process.pLoadAddress;
        incrementPC = false;
      }
      break;

    case BEQ: // BRANCH if Z == 0 WORKS
      byteOffset = (int32_t)(((uint32_t)ram.mem[PC + 4][0] << 24 |
                              (uint32_t)ram.mem[PC + 3][0] << 16 |
                              (uint32_t)ram.mem[PC + 2][0] << 8 |
                              (uint32_t)ram.mem[PC + 1][0]));
      if (Z == 0) {
        PC = byteOffset + process.pLoadAddress;
        incrementPC = false;
      }
      break;
    // LOGICAL //
    case CMP: // CMP <Reg1> - <Reg2>
      Z = Reg[ram.mem[PC + 1][0]] - Reg[ram.mem[PC + 2][0]];
      break;

    case AND:
      break;

    case ORR:
      break;

    case EOR:
      break;

    case SWI: // SWI 10 = HALT
      swiOpCode = (int32_t)(((uint32_t)ram.mem[PC + 4][0] << 24 |
                             (uint32_t)ram.mem[PC + 3][0] << 16 |
                             (uint32_t)ram.mem[PC + 2][0] << 8 |
                             (uint32_t)ram.mem[PC + 1][0]));
      switch (swiOpCode) {
      case 1: // PRINT
        printf("[VM] Returning 21\n");
        return 21;
      case 2: // INPUT
        return 22;
      case 10: // HALT
        return 0;
      }
      break;
    }

    if (incrementPC) {
      PC += 6;
    }

    clock++;
  }
  process.pKernelMode = 0;
  return 0;
}
