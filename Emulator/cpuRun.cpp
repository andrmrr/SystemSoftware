#include "emulator.h"

void CPU::run(){
  Word* pc = gpr+15;
  Word* sp = gpr+14;
  Word* status = csr+0;
  Word* handler = csr+1;
  Word* cause = csr+2;
  state = CPU::running;

  Word currInstruction;
  unsigned char opcode, a, b, c;
  int16_t* signedDisp;
  uint16_t* disp = (uint16_t*)malloc(sizeof(uint16_t));

  *pc += 4;
  currInstruction = readWord(*pc-4);
  while(state == CPU::running){
    opcode = currInstruction >> 24;
    a = (currInstruction >> 20) & 0xF;
    b = (currInstruction >> 16) & 0xF;
    c = (currInstruction >> 12) & 0xF;
    if(((currInstruction >> 11) & 1) == 0){ //pozitivan
      *disp = (currInstruction & 0x0FFF);
      signedDisp = (int16_t*)disp;
    } else { //negativan
      *disp = 0xF000 | (currInstruction & 0x0FFF);
      signedDisp = (int16_t*)disp;
    }
    Instruction instr(a, b, c, *signedDisp);

    switch(opcode){
      case 0x00: //HALT
        state = CPU::finished;
        break;
      case 0x10: //INT
        //TODO
        break;
      case 0x11: //IRET
        //TODO
        break;
      case 0x21: //CALL
        execCall(instr);
        break;
      case 0x38: //JMP
        execJmp(instr);
        break;
      case 0x39: //BEQ
        execBeq(instr);
        break;
      case 0x3a: //BNE
        execBne(instr);
        break;
      case 0x3b: //BGT
        execBgt(instr);
        break;
      case 0x40: //XCHG
        execXchg(instr);
        break;
      case 0x50: //ADD
        execAdd(instr);
        break;
      case 0x51: //SUB
        execSub(instr);
        break;
      case 0x52: //MUL
        execMul(instr);
        break;
      case 0x53: //DIV
        execDiv(instr);
        break;
      case 0x60: //NOT
        execNot(instr);
        break;
      case 0x61: //AND
        execAnd(instr);
        break;
      case 0x62: //OR
        execOr(instr);
        break;
      case 0x63: //XOR
        execXor(instr);
        break;
      case 0x70: //SHL
        execShl(instr);
        break;
      case 0x71: //SHR
        execShr(instr);
        break;
      case 0x80: //STMEM
        execStmem(instr);
        break;
      case 0x81: //PUSH
        execPush(instr);
        break;
      case 0x82: //STMEMMEM
        execStmemmem(instr);
        break;
      case 0x90: //CSRRD
        execCsrrd(instr);
        break;
      case 0x91: //REGREG
        execRegReg(instr);
        break;
      case 0x92: //LDMEM
        execLdmem(instr);
        break;
      case 0x93: //POP
        execPop(instr);
        break;
      case 0x94: //CSRWR
        execCsrwr(instr);
        break;

    }

    *pc += 4;
    currInstruction = readWord(*pc-4);
  }

  free(disp);
}

void CPU::execCall(Instruction instr){
  Instruction pushInstr(SP, 0, PC, -4);
  execPush(pushInstr);
  gpr[PC] = readWord(gpr[instr.a] + gpr[instr.b] + instr.d) ;
}

void CPU::execJmp(Instruction instr){
  gpr[PC] = readWord(gpr[instr.a] + instr.d);
}

void CPU::execBeq(Instruction instr){
  if(gpr[instr.b] == gpr[instr.c])
    gpr[PC] = readWord(gpr[instr.a] + instr.d);
}

void CPU::execBne(Instruction instr){
  if(gpr[instr.b] != gpr[instr.c])
    gpr[PC] = readWord(gpr[instr.a] + instr.d);
}

void CPU::execBgt(Instruction instr){
  if(gpr[instr.b] > gpr[instr.c])
    gpr[PC] = readWord(gpr[instr.a] + instr.d);
}

void CPU::execPush(Instruction instr){
  gpr[instr.a] = gpr[instr.a] + instr.d;
  writeWord(gpr[instr.a], gpr[instr.c]);
}

void CPU::execPop(Instruction instr){
  gpr[instr.a] = readWord(gpr[instr.b]);
  gpr[instr.b] = gpr[instr.b] + instr.d;
}


void CPU::execXchg(Instruction instr){
  Word temp = gpr[instr.b];
  gpr[instr.b] = gpr[instr.c];
  gpr[instr.c] = temp;
}

void CPU::execAdd(Instruction instr){
  gpr[instr.a] = gpr[instr.b] + gpr[instr.c];
}

void CPU::execSub(Instruction instr){
  gpr[instr.a] = gpr[instr.b] - gpr[instr.c];
}

void CPU::execMul(Instruction instr){
  gpr[instr.a] = gpr[instr.b] * gpr[instr.c];
}

void CPU::execDiv(Instruction instr){
  gpr[instr.a] = gpr[instr.b] / gpr[instr.c];
}

void CPU::execNot(Instruction instr){
  gpr[instr.a] = ~gpr[instr.b];
}

void CPU::execAnd(Instruction instr){
  gpr[instr.a] = gpr[instr.b] & gpr[instr.c];
}

void CPU::execOr(Instruction instr){
  gpr[instr.a] = gpr[instr.b] | gpr[instr.c];
}

void CPU::execXor(Instruction instr){
  gpr[instr.a] = gpr[instr.b] ^ gpr[instr.c];
}

void CPU::execShl(Instruction instr){
  gpr[instr.a] = gpr[instr.b] << gpr[instr.c];
}

void CPU::execShr(Instruction instr){
  gpr[instr.a] = gpr[instr.b] >> gpr[instr.c];
}

void CPU::execStmem(Instruction instr){
  writeWord(gpr[instr.a] + gpr[instr.b] + instr.d, gpr[instr.c]);
}

void CPU::execStmemmem(Instruction instr){
  writeWord(readWord(gpr[instr.a] + gpr[instr.b] + instr.d), gpr[instr.c]);
}

void CPU::execRegReg(Instruction instr){
  gpr[instr.a] = gpr[instr.b] + instr.d;
}

void CPU::execLdmem(Instruction instr){
  gpr[instr.a] = readWord(gpr[instr.b] + gpr[instr.c] + instr.d);
}

void CPU::execCsrrd(Instruction instr){
  gpr[instr.a] = csr[instr.b];
}

void CPU::execCsrwr(Instruction instr){
  csr[instr.a] = gpr[instr.b];
}
