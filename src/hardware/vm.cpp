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

void VirtualMachine::idle() { clock++; }

int VirtualMachine::fetchDecodeExecute(Pcb &process) {
  int opcode;

  // WEIRD VARIABLES THAT WOULD BE GOOD TO GET RID OF
  int strValue;
  int strAddress;

  int loadValue;
  int loadAddress;
  // printf("[VM] PC -- %d\n", PC);

  // CPU PREPERATION//
  bool incrementPC = true;

  opcode = ram.mem[PC][0];
  incrementPC = true;

  switch (opcode) {
  // ARITHMATIC //
  case ADD: // ADDS Reg<1> <-- Reg<2> + Reg<3>
    // printf("[VM] ADDING -- R[%d]\n", ram.mem[PC + 1][0]);
    Reg[ram.mem[PC + 1][0]] = Reg[ram.mem[PC + 2][0]] + Reg[ram.mem[PC + 3][0]];
    break;
  case SUB: // SUB Reg<1> <-- Reg<2> - Reg<3>
    // printf("[VM] SUBTRACTING -- R[%d]\n", ram.mem[PC + 1][0]);
    Reg[ram.mem[PC + 1][0]] = Reg[ram.mem[PC + 2][0]] + Reg[ram.mem[PC + 3][0]];
    break;
  case MUL: // MUL Reg<1> <-- Reg<2> * Reg<3>
    // printf("[VM] MULTIPLYING -- R[%d]\n", ram.mem[PC + 1][0]);
    Reg[ram.mem[PC + 1][0]] = Reg[ram.mem[PC + 2][0]] * Reg[ram.mem[PC + 3][0]];
    break;
  case DIV: // DIV Reg<1> <-- Reg<2> / Reg<3>
    // printf("[VM] DIVIDING -- R[%d]\n", ram.mem[PC + 1][0]);
    Reg[ram.mem[PC + 1][0]] = Reg[ram.mem[PC + 2][0]] / Reg[ram.mem[PC + 3][0]];
    break;
  case MOV: // MOVE Reg<1> <-- Reg<2>
    Reg[ram.mem[PC + 1][0]] = Reg[ram.mem[PC + 2][0]];
    break;
  case MVI: // MOVE IMM Reg<1> <-- Reg<2> | Reg<3> |
    // printf("[VM] MVI -- R[%d]\n", ram.mem[PC + 1][0]);
    Reg[ram.mem[PC + 1][0]] = (int32_t)((uint32_t)ram.mem[PC + 5][0] << 24 |
                                        (uint32_t)ram.mem[PC + 4][0] << 16 |
                                        (uint32_t)ram.mem[PC + 3][0] << 8 |
                                        (uint32_t)ram.mem[PC + 2][0]);
    break;
  case ADR:
    byteOffset = (int32_t)(uint32_t)ram.mem[PC + 5][0] << 24 |
                 (uint32_t)ram.mem[PC + 4][0] << 16 |
                 (uint32_t)ram.mem[PC + 3][0] << 8 |
                 (uint32_t)ram.mem[PC + 2][0];

    Reg[ram.mem[PC + 1][0]] = byteOffset + process.pLoadAddress;

    // printf("[ADR] [REG : %d] = %d\n", ram.mem[PC + 1][0],
    //        byteOffset + process.pLoadAddress);

    break;

  case STR: // memory[<reg2>] <reg1>
    strValue = Reg[ram.mem[PC + 1][0]];
    strAddress = ram.mem[Reg[ram.mem[PC + 2][0]]][0];
    ram.mem[strAddress][0] = strValue;
    // printf("ADDRESS:[%d] = %d\n", strAddress, strValue);
    break;
  case STRB:
    break;
  case LDR: // IDK IF THIS WORKS <reg1> <--memory[<reg2>]
    loadAddress = ram.mem[Reg[PC + 2]][0];
    loadValue = (int32_t)(uint32_t)ram.mem[loadAddress + 3][0] << 24 |
                (uint32_t)ram.mem[loadAddress + 2][0] << 16 |
                (uint32_t)ram.mem[loadAddress + 1][0] << 8 |
                (uint32_t)ram.mem[loadAddress][0];
    Reg[PC + 1] = loadValue;
    break;
  case LDRB: // <reg1> <-- byte(memory[<reg2>]

    loadValue = ram.mem[Reg[ram.mem[PC + 2][0]]][0];
    Reg[ram.mem[PC + 1][0]] = loadValue;
    // printf("[LDRB] ---> %d\n", loadValue);
    // printf("[LDRB][REG %d] = %d\n", PC + 1, Reg[0]);
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

  case BNE: // BRANCH if z != 0
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
    // printf("[CMP] Z = %d\n", Z);
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
      PC += 6;
      clock++;
      return 21;
    case 2: // INPUT
      PC += 6;
      clock++;
      // printf("[VM] Returning 22\n");
      return 22;
    case 3: // FORK
      PC += 6;
      clock++;
      // printf("[VM] Returning 23\n");
      return 23;
    case 10: // HALT
      PC += 6;
      clock++;
      return 10;
    }
  }

  clock++;

  if (incrementPC) {
    PC += 6;
  }

  if (PC >= process.pSize + process.pLoadAddress) {
    return 10;
  }

  return 0;
}
