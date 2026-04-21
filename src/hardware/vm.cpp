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
void VirtualMachine::setVerbosityFlag() {
  if (verbosityFlag == true) {
    verbosityFlag = false;
    return;
  }

  verbosityFlag = true;
  return;
}

void VirtualMachine::idle() { clock++; }

int VirtualMachine::fetchDecodeExecute(Pcb &process) {
  int opcode;
  uint8_t byteArray[4];

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
  clock++;

  switch (opcode) {
  // ARITHMATIC //
  case ADD: // ADDS Reg<1> <-- Reg<2> + Reg<3>
    Reg[ram.mem[PC + 1][0]] = Reg[ram.mem[PC + 2][0]] + Reg[ram.mem[PC + 3][0]];
    if (verbosityFlag == true) {
      std::cout << "[ADD] R[" << ram.mem[PC + 1][0] << "] <-- "
                << Reg[ram.mem[PC + 2][0]] << "+" << Reg[ram.mem[PC + 3][0]]
                << "\n";
    }
    break;
  case SUB: // SUB Reg<1> <-- Reg<2> - Reg<3>
    // printf("[VM] SUBTRACTING -- R[%d]\n", ram.mem[PC + 1][0]);
    Reg[ram.mem[PC + 1][0]] = Reg[ram.mem[PC + 2][0]] - Reg[ram.mem[PC + 3][0]];
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
    strValue = (int32_t)((uint32_t)ram.mem[PC + 5][0] << 24 |
                         (uint32_t)ram.mem[PC + 4][0] << 16 |
                         (uint32_t)ram.mem[PC + 3][0] << 8 |
                         (uint32_t)ram.mem[PC + 2][0]);

    Reg[ram.mem[PC + 1][0]] = strValue;

    if (verbosityFlag == true)
      std::cout << "[MVI] R[" << std::to_string(ram.mem[PC + 1][0]) << "] <--"
                << strValue << "\n";
    break;
  case ADR: // <reg1> <-- address(<label>)
    byteOffset = (int32_t)(uint32_t)ram.mem[PC + 5][0] << 24 |
                 (uint32_t)ram.mem[PC + 4][0] << 16 |
                 (uint32_t)ram.mem[PC + 3][0] << 8 |
                 (uint32_t)ram.mem[PC + 2][0];

    Reg[ram.mem[PC + 1][0]] = byteOffset + process.pLoadAddress;

    if (verbosityFlag == true)
      std::cout << "[ADR] R[" << std::to_string(ram.mem[PC + 1][0]) << "] <-- ["
                << std::hex << byteOffset + process.pLoadAddress << "]\n";

    break;

  case STR: // memory[<reg2>] <- <reg1>
    strValue = Reg[ram.mem[PC + 1][0]];
    strAddress = Reg[ram.mem[PC + 2][0]];

    byteArray[0] = (strValue >> 0) & 0xFF;
    byteArray[1] = (strValue >> 8) & 0xFF;
    byteArray[2] = (strValue >> 16) & 0xFF;
    byteArray[3] = (strValue >> 24) & 0xFF;

    for (int i = 0; i < 4; i++) {
      if (ram.mem[strAddress + i][1] == 1)
        ram.mem[strAddress + i][0] = byteArray[i];
      else {
        std::cout << "[VM]::str - pId::" << std::to_string(process.pId)
                  << " segfault\n";
        return 10;
      }
    }
    if (verbosityFlag == true)
      std::cout << process.pId << "[STR] [" << std::hex << strAddress
                << "] <-- " << strValue << std::endl;
    break;
  case STRB: // memory[<reg 2>] <-byte(memory[<reg1>])
    break;
  case LDR: // IDK IF THIS WORKS <reg1> <--memory[<reg2>]

    loadAddress = Reg[ram.mem[PC + 2][0]];
    loadValue = (int32_t)(((uint32_t)ram.mem[loadAddress + 3][0] << 24 |
                           (uint32_t)ram.mem[loadAddress + 2][0] << 16 |
                           (uint32_t)ram.mem[loadAddress + 1][0] << 8 |
                           (uint32_t)ram.mem[loadAddress][0]));
    if (ram.mem[loadAddress][1] == 0) {
      std::cout << process.pId << "[VM]::ldr - pName::[" << process.name
                << "] addr::[" << std::hex << loadAddress << "] - segfault\n";
      return 10;
    }

    Reg[ram.mem[PC + 1][0]] = loadValue;

    if (verbosityFlag == true)
      std::cout << process.pId << "[LDR] R["
                << std::to_string(ram.mem[PC + 1][0]) << "] <-- [" << loadValue
                << "]\n";
    break;

  case LDRB: // <reg1> <-- byte(memory[<reg2>]
    loadValue = ram.mem[Reg[ram.mem[PC + 2][0]]][0];
    Reg[ram.mem[PC + 1][0]] = loadValue;
    // printf("[LDRB] ---> %d\n", loadValue);
    //
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
    case 0: // PRINT INT
      PC += 6;
      clock++;
      if (verbosityFlag == true)
        std::cout << process.pId << "[SWI] 0\n";
      return 20;

    case 1: // PRINT
      PC += 6;
      clock++;
      if (verbosityFlag == true)
        std::cout << process.pId << "[SWI] 1\n";
      return 21;

    case 2: // INPUT
      PC += 6;
      clock++;
      if (verbosityFlag == true)
        std::cout << process.pId << "[SWI] 2\n";
      return 22;

    case 3: // FORK
      PC += 6;
      clock++;
      if (verbosityFlag == true)
        std::cout << process.pId << "[SWI] 3\n";
      return 23;

    case 4: // RETURN SHARED MEMORY POINTER
      PC += 6;
      clock++;
      if (verbosityFlag == true)
        std::cout << process.pId << "[SWI] 4\n";
      return 24;

    case 5: // AQUIRE SEMAPHORE
      PC += 6;
      clock++;
      if (verbosityFlag == true)
        std::cout << process.pId << "[SWI] 5\n";
      return 25;

    case 6: // RELEASE SEMAPHORE
      PC += 6;
      clock++;
      if (verbosityFlag == true)
        std::cout << process.pId << "[SWI] 6\n";
      return 26;

    case 7: // WAIT SEMAPHORE
      PC += 6;
      clock++;
      if (verbosityFlag == true)
        std::cout << process.pId << "[SWI] 7\n";
      return 27;

    case 10: // HALT
      PC += 6;
      clock++;
      if (verbosityFlag == true)
        std::cout << process.pId << "[SWI] 10\n";
      return 10;
    }
  }

  if (incrementPC) {
    PC += 6;
  }

  // if (PC > process.pSize + process.pLoadAddress) {
  //   return 10;
  // }
  //
  return 0;
}
