#include "asembler.h"
#include <iostream>

void Asembler::secondPassInit(){
  SectionTable::initSectionTable();
  secTable = SectionTable::getInstance();
  vector<Symbol*> secSymbols = symbolTable->getSections();
  for(auto it = secSymbols.begin(); it != secSymbols.end(); it++){
    secTable->addSection(*it);
    relTables.push_back(new RelocationTable(*it));
  }
}

void Asembler::write(char* chars, int size){
  secTable->writeToSection(currSection, chars, size);
  incCounter(size);
}

void Asembler::fill(char filler, int size){
  secTable->fillSection(currSection, filler, size);
  incCounter(size);
}

void Asembler::secondPass(){
  bool printInst = false;
  secondPassInit();
  int tokenCnt = 0;
  Token token, nextToken;
  bool syntaxError = false;
  uint32_t temp32;
  string strTemp;
  char* cTemp = (char*) calloc(10, sizeof(char));

  while(tokenCnt < tokens.size()){
    token = tokens[tokenCnt++];

    // if(printInst) cout << "Token " << token.getType() << endl;
    *cTemp = Token::getOPCodeBinary(token.getType());
    switch(token.getType()) {
      case TokenType::HALT:
        if(printInst) cout << "halt" << endl;
        write(cTemp, 1);
        break;
      case TokenType::INT:
        if(printInst) cout << "int" << endl;
        write(cTemp, 1);
        break;
      case TokenType::IRET:
        if(printInst) cout << "iret" << endl;
        write(cTemp, 1);
        break;
      case TokenType::CALL: 
        if(printInst) cout << "call" << endl;
        syntaxError = handleBranchOperand(&tokenCnt, cTemp, 1) ? false : true;
        write(cTemp, 5);
        break;
      case TokenType::RET:
        if(printInst) cout << "ret " << endl;
        write(cTemp, 1);
        break;
      case TokenType::JMP: 
        if(printInst) cout << "jmp" << endl;
        syntaxError = handleBranchOperand(&tokenCnt, cTemp, 1) ? false : true;
        write(cTemp, 5);
        break;
      case TokenType::BEQ:
        if(printInst) cout << "beq" << endl;
        handleCondition(&tokenCnt, cTemp);
        syntaxError = handleBranchOperand(&tokenCnt, cTemp, 2) ? false : true;
        write(cTemp, 6);
        break;
      case TokenType::BNE:
        if(printInst) cout << "bne" << endl;
        handleCondition(&tokenCnt, cTemp);
        syntaxError = handleBranchOperand(&tokenCnt, cTemp, 2) ? false : true;
        write(cTemp, 6);
        break;
      case TokenType::BGT:
        if(printInst) cout << "bgt" << endl;
        handleCondition(&tokenCnt, cTemp);
        syntaxError = handleBranchOperand(&tokenCnt, cTemp, 2) ? false : true;
        write(cTemp, 6);
        break;
      case TokenType::PUSH:
        if(printInst) cout << "push" << endl;
        handle1gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::POP:
        if(printInst) cout << "pop" << endl;
        handle1gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::XCHG:
        if(printInst) cout << "xchg" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::ADD:
        if(printInst) cout << "add" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::SUB:
        if(printInst) cout << "sub" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::MUL: 
        if(printInst) cout << "mul" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::DIV: 
        if(printInst) cout << "div" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::NOT: 
        if(printInst) cout << "not" << endl;
        handle1gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::AND: 
        if(printInst) cout << "and" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::OR: 
        if(printInst) cout << "or" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::XOR: 
        if(printInst) cout << "xor" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::SHL: 
        if(printInst) cout << "shl" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::SHR: 
        if(printInst) cout << "shr" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::LDMEMDIR:
        if(printInst) cout << "ldmemdir" << endl;
        syntaxError = handle1doperand1gpr(&tokenCnt, cTemp) ? false : true;
        write(cTemp, 6);
        break;
      case TokenType::LDREGDIR:
        if(printInst) cout << "ldregdir" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::LDREGIND:
        if(printInst) cout << "ldregind" << endl;
        handle1indir1gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::LDREGINDADD:
        if(printInst) cout << "ldregindadd" << endl;
        syntaxError = handle1indirAddend1gpr(&tokenCnt, cTemp) ? false : true;
        write(cTemp, 4);
        break;
      case TokenType::LDIMMED:
        if(printInst) cout << "ldimmed" << endl;
        syntaxError = handle1lit1gpr(&tokenCnt, cTemp) ? false : true;
        write(cTemp, 6);
        break;
      case TokenType::STMEMDIR: 
        if(printInst) cout << "stmemdir" << endl;
        syntaxError = handle1gpr1doperand(&tokenCnt, cTemp) ? false : true;
        write(cTemp, 6);
        break;
      case TokenType::STREGDIR:
        if(printInst) cout << "stregdir" << endl;
        handle2gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::STREGIND:
        if(printInst) cout << "stregind" << endl;
        handle1gpr1indir(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::STREGINDADD:
        if(printInst) cout << "stregindadd" << endl;
        syntaxError = handle1gpr1indirAddend(&tokenCnt, cTemp) ? false : true;
        write(cTemp, 4);
        break;
      case TokenType::CSRRD:  
        if(printInst) cout << "csrrd" << endl;
        handle1csr1gpr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::CSRWR: 
        if(printInst) cout << "csrwr" << endl;
        handle1gpr1csr(&tokenCnt, cTemp);
        write(cTemp, 2);
        break;
      case TokenType::GLOBAL: 
        if(printInst) cout << "global" << endl;
        syntaxError = handleGlobal(&tokenCnt) ? false : true;
        break;
      case TokenType::SECTION:
        if(printInst) cout << "section" << endl;
        resetCounter();
        nextToken = tokens[tokenCnt++];
        currSection = symbolTable->findSymbol(nextToken.getText());
        break;
      case TokenType::WORD:
        if(printInst) cout << "word" << endl;
        syntaxError = handleWord(&tokenCnt, cTemp) ? false : true;
        break;
      case TokenType::SKIP:
        if(printInst) cout << "skip" << endl;
        handleSkip(&tokenCnt);
        break;
      case TokenType::ASCII:
        if(printInst) cout << "ascii" << endl;
        handleAscii(&tokenCnt);
        break;
      // case TokenType::EQU:
      //   if(printInst) cout << "equ dobar!" << endl;
      //   syntaxError = checkExpr(&tokenCnt) ? false : true;
      //   break;
      case TokenType::END:
        if(printInst) cout << "end" << endl;
        return;
      default: break;
    }

    if(syntaxError){
      //uradi nesto
      //if(printInst) cout << "GRESKA PRI PARSIRANJU PRI DRUGOM PROLAZU" << endl;
      error = true;
      break;
    }
  }
  free(cTemp);
  if(printInst) cout << "Opet cisto reda radi" << endl;
}

string Asembler::int12ToHexNoPrefix(int num){
  if(num > 2^11-1 || num < -2^11) return "";

  string hex = "000";
  int rem, i = 0;
  while(num > 0){
    rem = num % 16;
    switch(rem){
      case 0:
        hex[i] = '0';
        break;
      case 1:
        hex[i] = '1';
        break;
      case 2:
        hex[i] = '2';
        break;
      case 3:
        hex[i] = '3';
        break;
      case 4:
        hex[i] = '4';
        break;
      case 5:
        hex[i] = '5';
        break;
      case 6:
        hex[i] = '6';
        break;
      case 7:
        hex[i] = '7';
        break;
      case 8:
        hex[i] = '8';
        break;
      case 9:
        hex[i] = '9';
        break;
      case 10:
        hex[i] = 'a';
        break;
      case 11:
        hex[i] = 'b';
        break;
      case 12:
        hex[i] = 'c';
        break;
      case 13:
        hex[i] = 'd';
        break;
      case 14:
        hex[i] = 'e';
        break;
      case 15:
        hex[i] = 'f';
        break;
    }
    num /= 16;
    i--;
  }
  return hex;
}

int Asembler::addRelocationPCREL(Symbol* s, int offset){
  int offs = lcounter + offset;
  int addend = -4;
  Symbol* relSym = s;
  if(!(s->isGlobal())){
    addend += s->getValue();
    relSym = symbolTable->findSymbol(s->getSection());
  }

  //ako smo u istoj sekciji
  if(relSym->getSection() == currSection->getSection()){
        return relSym->getValue() - offs + addend;
  }

  for(auto it = relTables.begin(); it != relTables.end(); it++){
    if((*it)->getSection()->getSection() == currSection->getSection()){
      (*it)->addRelative(offs, relSym->getName(), addend);
      return 0;
    }
  }
  return -1;
}

int Asembler::addRelocationABS(Symbol* s, int offset){
  int offs = lcounter + offset;
  int addend = 0;
  Symbol* relSym = s;
  if(!(s->isGlobal())){
    addend += s->getValue();
    relSym = symbolTable->findSymbol(s->getSection());
  }

  for(auto it = relTables.begin(); it != relTables.end(); it++){
    if((*it)->getSection()->getSection() == currSection->getSection()){
      (*it)->addAbsolute(offs, relSym->getName(), addend);
      return 0;
    }
  }
  return -1;
}

bool Asembler::handleBranchOperand(int* tokenCnt, char* charr, int addrStart){
  uint32_t operand = 0;
  Token nextToken = tokens[(*tokenCnt)++];
  if(nextToken.getType() == TokenType::HEX){
    operand = hexStringToInt(nextToken.getText(), true);
  } else if(nextToken.getType() == TokenType::DEC){
    operand = atoi(nextToken.getText().c_str());
  } else { //simbol
    Symbol* s = symbolTable->findSymbol(nextToken.getText());
    if(s == nullptr){
      cout << "Unidentified symbol!" << endl;
      return false;
    }
    if(s->isAbsolute()){
      operand = s->getValue();
    } else {
      operand = addRelocationABS(s, addrStart);
    }
  }
  charr[addrStart++] = *((char*)&operand+0); //little-endian
  charr[addrStart++] = *((char*)&operand+1);
  charr[addrStart++] = *((char*)&operand+2);
  charr[addrStart++] = *((char*)&operand+3);
  return true;
}

void Asembler::handleCondition(int* tokenCnt, char* charr) {
  //gpr1, gpr2,
  handle2gpr(tokenCnt, charr);
  (*tokenCnt)++; // pojedi zapetu
}

void Asembler::handle1gpr(int* tokenCnt, char* charr){
  //gpr
  string gpr;
  char ind;
  gpr = tokens[(*tokenCnt)++].getText();
  ind = gprIndex(gpr);
  *(charr+1) = (0x0f & ind);
}

void Asembler::handle2gpr(int* tokenCnt, char* charr){
  //gprS, gprD
  string gprS, gprD;
  char indS, indD, regs;
  gprS = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  gprD = tokens[(*tokenCnt)++].getText();
  indS = gprIndex(gprS);
  indD = gprIndex(gprD);
  regs = ((indD << 4) | (0x0f & indS));
  *(charr+1) = regs;
}

char Asembler::gprIndex(string gpr){
  if(gpr.compare("%sp") == 0){
    return 14;
  } else if(gpr.compare("%pc") == 0){
    return 15;
  } else {
    return atoi(gpr.substr(2).c_str());
  }
  return -1;
}

bool Asembler::handle1doperand1gpr(int* tokenCnt, char* charr){
  //sym, gpr
  bool status = handleDataOperand(tokenCnt, charr);
  (*tokenCnt)++; // pojedi zapetu
  handle1gpr(tokenCnt, charr);
  return status;
}

void Asembler::handle1indir1gpr(int* tokenCnt, char* charr){
  // [%gprS], gprD
  string gprS, gprD;
  char indS, indD, regs;
  (*tokenCnt)++; // pojedi [
  gprS = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi ]
  (*tokenCnt)++; // pojedi zapetu
  gprD = tokens[(*tokenCnt)++].getText();
  indS = gprIndex(gprS);
  indD = gprIndex(gprD);
  regs = ((indD << 4) | (0x0f & indS));
  *(charr+1) = regs;
}
bool Asembler::handle1indirAddend1gpr(int* tokenCnt, char* charr){
  //[%gprS + sym], %gprD
  bool status;
  string gprS, gprD;
  char indS, indD, regs;
  (*tokenCnt)++; // pojedi [
  gprS = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi +
  status = handle12bitOperand(tokenCnt, charr);
  (*tokenCnt)++; // pojedi ]
  (*tokenCnt)++; // pojedi zapetu
  gprD = tokens[(*tokenCnt)++].getText();
  indS = gprIndex(gprS);
  indD = gprIndex(gprD);
  regs = ((indD << 4) | (0x0f & indS));
  *(charr+1) = regs;
  return status;
}

bool Asembler::handle12bitOperand(int* tokenCnt, char* charr){
  int operand = 0;
  Token nextToken = tokens[(*tokenCnt)++];
  if(nextToken.getType() == TokenType::HEX){
    operand = hexStringToInt(nextToken.getText(), true);
  } else if(nextToken.getType() == TokenType::DEC){
    operand = atoi(nextToken.getText().c_str());
  } else { //simbol
    Symbol* s = symbolTable->findSymbol(nextToken.getText());
    if(s == nullptr || s->getSection() != 1){
      cout << "Unidentified symbol!" << endl;
      return false;
    }
    operand = s->getValue();
  }

  if(operand > (1<<12)) {
    cout << "Operand is larger than 12 bits!" << endl;
    return false;
  } else {
    charr[2] = *((char*)&operand+0); //little-endian
    charr[3] = *((char*)&operand+1);
  }
  return true;
}

bool Asembler::handle1lit1gpr(int* tokenCnt, char* charr){
  //$sym, %gprD
  bool status;
  string symbolOnly = tokens[*tokenCnt].getText();
  symbolOnly = symbolOnly.substr(1);
  tokens[*tokenCnt].setText(symbolOnly);
  status = handleDataOperand(tokenCnt, charr);
  (*tokenCnt)++; // pojedi zapetu
  handle1gpr(tokenCnt, charr);
  return status;
}

bool Asembler::handleDataOperand(int* tokenCnt, char* charr){
  int addrStart = 2;
  uint32_t operand = 0;
  Token nextToken = tokens[(*tokenCnt)++];
  if(nextToken.getType() == TokenType::HEX){
    operand = hexStringToInt(nextToken.getText(), true);
  } else if(nextToken.getType() == TokenType::DEC){
    operand = atoi(nextToken.getText().c_str());
  } else { //simbol
    Symbol* s = symbolTable->findSymbol(nextToken.getText());
    if(s == nullptr){
      cout << "Unidentified symbol!" << endl;
      return false;
    }
    if(s->isAbsolute()) {
      operand = s->getValue();
    } else {
      operand = addRelocationABS(s, addrStart);
    }
  }
  charr[addrStart++] = *((char*)&operand+0); //little-endian
  charr[addrStart++] = *((char*)&operand+1);
  charr[addrStart++] = *((char*)&operand+2);
  charr[addrStart++] = *((char*)&operand+3);
  return true;
}

bool Asembler::handle1gpr1doperand(int*tokenCnt, char* charr){
  //gpr, sym
  handle1gpr(tokenCnt, charr);
  (*tokenCnt)++; // pojedi zapetu
  bool status = handleDataOperand(tokenCnt, charr);
  return status;
}

void Asembler::handle1gpr1indir(int*tokenCnt, char* charr){
   // gprS, [%gprD]
  string gprS, gprD;
  char indS, indD, regs;
  gprS = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  (*tokenCnt)++; // pojedi [
  gprD = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi ]
  indS = gprIndex(gprS);
  indD = gprIndex(gprD);
  regs = ((indD << 4) | (0x0f & indS));
  *(charr+1) = regs;
}

bool Asembler::handle1gpr1indirAddend(int*tokenCnt, char* charr){
  // %gprS, [%gprD + sym]
  bool status;
  string gprS, gprD;
  char indS, indD, regs;
  gprS = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  (*tokenCnt)++; // pojedi [
  gprD = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi +
  status = handle12bitOperand(tokenCnt, charr);
  (*tokenCnt)++; // pojedi ]
  indS = gprIndex(gprS);
  indD = gprIndex(gprD);
  regs = ((indD << 4) | (0x0f & indS));
  *(charr+1) = regs;
  return status;
}

char Asembler::csrIndex(string csr){
  if(csr.compare("%status") == 0){
    return 0;
  } else if(csr.compare("%handler") == 0){
    return 1;
  } else if(csr.compare("%cause") == 0){
    return 2;
  } 
  return -1;
}

void Asembler::handle1gpr1csr(int* tokenCnt, char* charr){
  // gpr, csr
  string gpr, csr;
  char indGpr, indCsr, regs;
  gpr = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  csr = tokens[(*tokenCnt)++].getText();
  indGpr = gprIndex(gpr);
  indCsr = csrIndex(csr);
  regs = ((indCsr << 4) | (0x0f & indGpr));
  *(charr+1) = regs;
}
void Asembler::handle1csr1gpr(int* tokenCnt, char* charr){
  // csr, gpr
  string gpr, csr;
  char indCsr, indGpr, regs;
  csr = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  gpr = tokens[(*tokenCnt)++].getText();
  indCsr = csrIndex(csr);
  indGpr = gprIndex(gpr);
  regs = ((indGpr << 4) | (0x0f & indCsr));
  *(charr+1) = regs;
}

bool Asembler::handleGlobal(int* tokenCnt){
  //sym1, sym2, ... symN
  Token nextToken = tokens[(*tokenCnt)++];
  string name = nextToken.getText();
  Symbol* s;
  while(nextToken.getType() != TokenType::EOL) {
    s = symbolTable->findSymbol(name);
    if(s == nullptr) return false;
    s->setGlobal();

    nextToken = tokens[(*tokenCnt)++]; //pojedi zapetu
    if(nextToken.getType() == TokenType::EOL) break;
    nextToken = tokens[(*tokenCnt)++];
    name = nextToken.getText();
  }
  return true;
}

bool Asembler::handleWord(int* tokenCnt, char* charr){
  // sym1|lit1, sym2|lit2, ... symN|litN
  Token nextToken = tokens[(*tokenCnt)++];
  int val = 0;
  Symbol* s;
  while(nextToken.getType() != TokenType::EOL){
      if(nextToken.getType() == TokenType::DEC) {
        val = atoi(nextToken.getText().c_str());
      } else if(nextToken.getType() == TokenType::HEX){
        val = hexStringToInt(nextToken.getText(), true);
      } else { //simbol
        s = symbolTable->findSymbol(nextToken.getText());
        if(s == nullptr) return false;
        if(s->isAbsolute()) {
          val = s->getValue();
        } else {
          addRelocationABS(s, 0);
          val = 0;
        }
      }
  
    charr[0] = *((char*)&val+0); //little-endian
    charr[1] = *((char*)&val+1);
    charr[2] = *((char*)&val+2);
    charr[3] = *((char*)&val+3);
    write(charr, 4);
    nextToken = tokens[(*tokenCnt)++]; //pojedi zapetu
    nextToken = tokens[(*tokenCnt)++];
  }
  return true;
}

void Asembler::handleSkip(int* tokenCnt){
  Token nextToken = tokens[(*tokenCnt)++];
  int val;
  if(nextToken.getType() == TokenType::DEC){
    val = atoi(nextToken.getText().c_str());
  } else { //hex
    val = hexStringToInt(nextToken.getText(), true);
  }

  fill(0, val);
}

void Asembler::handleAscii(int* tokenCnt){
  Token nextToken = tokens[(*tokenCnt)++];
  string str = nextToken.getText();
  write(((char*)str.c_str()), str.size());
}

