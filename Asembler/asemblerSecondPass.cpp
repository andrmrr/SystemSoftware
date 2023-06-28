#include "asembler.h"
#include <iostream>

void Asembler::secondPassInit(){
  SectionTable::initSectionTable();
  secTable = SectionTable::getInstance();
  Pool::initPool();
  pool = Pool::getInstance();
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

void Asembler::writePool(){
  if(pool == nullptr) return;
  vector<char> data = secTable->findSection(currSection)->getData();
  uint32_t lit;
  string sym;
  uint16_t disp;
  char b1, b2;
  vector<uint32_t> offsets;
  //Literali
  while(1){
    offsets = pool->getLiteralOffsets();
    if(offsets.empty()) break;
    for(auto it = offsets.begin(); it != offsets.end(); it++){
      uint32_t offset = *it;
      b1 = data[offset];
      b2 = data[offset+1];
      //postavljamo 12-bitni pomeraj na kraj sekcije gde ce doci novi literal
      disp = data.size() - offset - 2;
      b1 = b1 | ((disp >> 8) & 0x0F);
      b2 = disp & 0xFF;
      data[offset] = b1;
      data[offset+1] = b2;
    }
    lit = pool->getLiteral();
    data.push_back(*((char*)(&lit)+3));
    data.push_back(*((char*)(&lit)+2));
    data.push_back(*((char*)(&lit)+1));
    data.push_back(*((char*)(&lit)+0));
  }
  //Simboli
  while(1){
    offsets = pool->getSymbolOffsets();
    if(offsets.empty()) break;
    for(auto it = offsets.begin(); it != offsets.end(); it++){
      uint32_t offset = *it;
      b1 = data[offset];
      b2 = data[offset+1];
      //postavljamo 12-bitni pomeraj na kraj sekcije gde ce doci novi simbol
      disp = data.size() - offset - 2;
      b1 = b1 | (disp >> 8) &0x0F;
      b2 = disp & 0xFF;
      data[offset] = b1;
      data[offset+1] = b2;
    }
    sym = pool->getSymbol();
    addRelocationABS(symbolTable->findSymbol(sym), data.size());
    data.push_back(0);
    data.push_back(0);
    data.push_back(0);
    data.push_back(0);
  }

  secTable->findSection(currSection)->setData(data);
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
  char* instr = (char*) malloc(4*sizeof(char));
  char opcode;

  while(tokenCnt < tokens.size()){
    token = tokens[tokenCnt++];

    // if(printInst) cout << "Token " << token.getType() << endl;
    switch(token.getType()) {
      case TokenType::HALT:
        if(printInst) cout << "halt" << endl;
        opcode = Token::getOPCodeBinary(token.getType());
        writeInstruction(instr, opcode, 0, 0, 0, 0);
        break;
      case TokenType::INT:
        if(printInst) cout << "int" << endl;
        opcode = Token::getOPCodeBinary(token.getType());
        writeInstruction(instr, opcode, 0, 0, 0, 0);
        break;
      case TokenType::IRET:
        if(printInst) cout << "iret" << endl;
        opcode = Token::getOPCodeBinary(token.getType());
        writeInstruction(instr, opcode, 0, 0, 0, 0);
        break;
      case TokenType::CALL: 
        if(printInst) cout << "call" << endl;
        syntaxError = handleCall(&tokenCnt, instr);
        break;
      case TokenType::RET:
        if(printInst) cout << "ret " << endl;
        handleRet(&tokenCnt, instr);
        break;
      case TokenType::JMP: 
        if(printInst) cout << "jmp" << endl;
        syntaxError = handleJmp(&tokenCnt, instr);
        break;
      case TokenType::BEQ:
        if(printInst) cout << "beq" << endl;
        syntaxError = handleBeq(&tokenCnt, instr);
        break;
      case TokenType::BNE:
        if(printInst) cout << "bne" << endl;
        syntaxError = handleBne(&tokenCnt, instr);
        break;
      case TokenType::BGT:
        if(printInst) cout << "bgt" << endl;
        syntaxError = handleBgt(&tokenCnt, instr);
        break;
      case TokenType::PUSH:
        if(printInst) cout << "push" << endl;
        handlePush(&tokenCnt, instr);
        break;
      case TokenType::POP:
        if(printInst) cout << "pop" << endl;
        handlePop(&tokenCnt, instr);
        break;
      case TokenType::XCHG:
        if(printInst) cout << "xchg" << endl;
        handleXchg(&tokenCnt, instr);
        break;
      case TokenType::ADD:
        if(printInst) cout << "add" << endl;
        handleAdd(&tokenCnt, instr);
        break;
      case TokenType::SUB:
        if(printInst) cout << "sub" << endl;
        handleSub(&tokenCnt, instr);
        break;
      case TokenType::MUL: 
        if(printInst) cout << "mul" << endl;
        handleMul(&tokenCnt, instr);
        break;
      case TokenType::DIV: 
        if(printInst) cout << "div" << endl;
        handleDiv(&tokenCnt, instr);
        break;
      case TokenType::NOT: 
        if(printInst) cout << "not" << endl;
        handleNot(&tokenCnt, instr);
        break;
      case TokenType::AND: 
        if(printInst) cout << "and" << endl;
        handleAnd(&tokenCnt, instr);
        break;
      case TokenType::OR: 
        if(printInst) cout << "or" << endl;
        handleOr(&tokenCnt, instr);
        break;
      case TokenType::XOR: 
        if(printInst) cout << "xor" << endl;
        handleXor(&tokenCnt, instr);
        break;
      case TokenType::SHL: 
        if(printInst) cout << "shl" << endl;
        handleShl(&tokenCnt, instr);
        break;
      case TokenType::SHR: 
        if(printInst) cout << "shr" << endl;
        handleShr(&tokenCnt, instr);
        break;
      case TokenType::LDMEMDIR:
        if(printInst) cout << "ldmemdir" << endl;
        syntaxError = handleLdmemdir(&tokenCnt, instr);
        break;
      case TokenType::LDREGDIR:
        if(printInst) cout << "ldregdir" << endl;
        handleLdregdir(&tokenCnt, instr);
        break;
      case TokenType::LDREGIND:
        if(printInst) cout << "ldregind" << endl;
        handleLdregind(&tokenCnt, instr);
        break;
      case TokenType::LDREGINDADD:
        if(printInst) cout << "ldregindadd" << endl;
        syntaxError = handleLdregindadd(&tokenCnt, instr);
        break;
      case TokenType::LDIMMED:
        if(printInst) cout << "ldimmed" << endl;
        syntaxError = handleLdimmed(&tokenCnt, instr);
        break;
      case TokenType::STMEMDIR: 
        if(printInst) cout << "stmemdir" << endl;
        syntaxError = handleStmemdir(&tokenCnt, instr);
        break;
      case TokenType::STREGDIR:
        if(printInst) cout << "stregdir" << endl;
        handleStregdir(&tokenCnt, instr);
        break;
      case TokenType::STREGIND:
        if(printInst) cout << "stregind" << endl;
        handleStregind(&tokenCnt, instr);
        break;
      case TokenType::STREGINDADD:
        if(printInst) cout << "stregindadd" << endl;
        syntaxError = handleStregindadd(&tokenCnt, instr);
        break;
      case TokenType::CSRRD:  
        if(printInst) cout << "csrrd" << endl;
        handleCsrrd(&tokenCnt, instr);
        break;
      case TokenType::CSRWR: 
        if(printInst) cout << "csrwr" << endl;
        handleCsrwr(&tokenCnt, instr);
        break;
      case TokenType::GLOBAL: 
        if(printInst) cout << "global" << endl;
        syntaxError = handleGlobal(&tokenCnt) ? false : true;
        break;
      case TokenType::SECTION:
        if(printInst) cout << "section" << endl;
        if (getCounter() > 0) {
          writePool();
          Pool::initPool();
          pool = Pool::getInstance();
        }
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
      case TokenType::EQU:
        if(printInst) cout << "equ" << endl;
        skipToNewLine(&tokenCnt);
        break;
      case TokenType::END:
        if(printInst) cout << "end" << endl;
        writePool();
        return;
      default: break;
    }

    if(syntaxError){
      //uradi nesto
      if(printInst) cout << "GRESKA PRI PARSIRANJU PRI DRUGOM PROLAZU" << endl;
      error = true;
      break;
    }
  }
  if(!error) writePool();
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
  // int offs = lcounter + offset;
  int addend = 0;
  Symbol* relSym = s;
  if(!(s->isGlobal())){
    addend += s->getValue();
    relSym = symbolTable->findSymbol(s->getSection());
  }

  for(auto it = relTables.begin(); it != relTables.end(); it++){
    if((*it)->getSection()->getSection() == currSection->getSection()){
      (*it)->addAbsolute(offset, relSym->getName(), addend);
      return 0;
    }
  }
  return -1;
}

bool Asembler::handleOperand(int* tokenCnt, int offsFromLC){
  uint32_t offset = getCounter() + offsFromLC; // da bismo dosli do displacement dela instrukcije
  uint32_t operand = 0;
  Token nextToken = tokens[(*tokenCnt)++];
  if(nextToken.getType() == TokenType::HEX || nextToken.getType() == TokenType::LIT_HEX){
    operand = hexStringToInt(nextToken.getText(), true);
    pool->addLiteral(operand, offset);
    return false;
  } else if(nextToken.getType() == TokenType::DEC || nextToken.getType() == TokenType::LIT_DEC){
    operand = atoi(nextToken.getText().c_str());
    pool->addLiteral(operand, offset);
    return false;
  } else { //simbol
    Symbol* s = symbolTable->findSymbol(nextToken.getText());
    if(s == nullptr){
      cout << "Unidentified symbol operand!" << endl;
      return true;
    }
    if(s->isAbsolute()){
      operand = s->getValue();
      pool->addLiteral(operand, offset);
    } else {
      pool->addSymbol(s->getName(), offset);
    }
    return false;
  }
}


void Asembler::handle1gpr(int* tokenCnt, char* gpr){
  string s;
  char ind;
  s = tokens[(*tokenCnt)++].getText();
  *gpr = gprIndex(s);
}

void Asembler::handle2gpr(int* tokenCnt, char* gpr1, char* gpr2){
  string s1, s2;
  s1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  s2 = tokens[(*tokenCnt)++].getText();
  *gpr1 = gprIndex(s1);
  *gpr2 = gprIndex(s2);
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

void Asembler::handle1indir1gpr(int* tokenCnt, char* gpr1, char* gpr2){
  string s1, s2;
  (*tokenCnt)++; // pojedi [
  s1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi ]
  (*tokenCnt)++; // pojedi zapetu
  s2 = tokens[(*tokenCnt)++].getText();
  *gpr1 = gprIndex(s1);
  *gpr2 = gprIndex(s2);
}


bool Asembler::handle1indirAddend1gpr(int* tokenCnt, char* gpr1, char* gpr2, uint16_t* disp){
  //[%gprS + sym], %gprD
  string s1, s2;
  (*tokenCnt)++; // pojedi [
  s1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi +
  bool status = handle12bitOperand(tokenCnt, disp);
  (*tokenCnt)++; // pojedi ]
  (*tokenCnt)++; // pojedi zapetu
  s2 = tokens[(*tokenCnt)++].getText();
  *gpr1 = gprIndex(s1);
  *gpr2 = gprIndex(s2);
  return status;
}

bool Asembler::handle12bitOperand(int* tokenCnt, uint16_t* disp){
  int operand = 0;
  Token nextToken = tokens[(*tokenCnt)++];
  if(nextToken.getType() == TokenType::HEX){
    operand = hexStringToInt(nextToken.getText(), true);
  } else if(nextToken.getType() == TokenType::DEC){
    operand = atoi(nextToken.getText().c_str());
  } else { //simbol
    Symbol* s = symbolTable->findSymbol(nextToken.getText());
    if(s == nullptr || s->getSection() != 1){
      cout << "Unidentified symbol displacement!" << endl;
      return true;
    }
    operand = s->getValue();
  }

  if(operand >= (1<<12)) {
    cout << "Operand is larger than 12 bits!" << endl;
    return true;
  } else {
    *disp = operand & 0xFFFF;
  }
  return false;
}

void Asembler::handle1gpr1indir(int* tokenCnt, char* gpr1, char* gpr2){
  string s1, s2;
  s1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  (*tokenCnt)++; // pojedi [
  s2 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi ]
  *gpr1 = gprIndex(s1);
  *gpr2 = gprIndex(s2);
}

bool Asembler::handle1gpr1indirAddend(int* tokenCnt, char* gpr1, char* gpr2, uint16_t* disp){
  // %gprS, [%gprD + sym]
  string s1, s2;
  s1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  (*tokenCnt)++; // pojedi [
  s2 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi +
  bool status = handle12bitOperand(tokenCnt, disp);
  (*tokenCnt)++; // pojedi ]
  *gpr1 = gprIndex(s1);
  *gpr2 = gprIndex(s2);
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

void Asembler::handle1gpr1csr(int* tokenCnt, char* gpr, char* csr){
  string s1, s2;
  s1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  s2 = tokens[(*tokenCnt)++].getText();
  *gpr = gprIndex(s1);
  *csr = csrIndex(s2);
}

void Asembler::handle1csr1gpr(int* tokenCnt, char* csr, char* gpr){
  string s1, s2;
  s1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  s2 = tokens[(*tokenCnt)++].getText();
  *csr = csrIndex(s1);
  *gpr = gprIndex(s2);
}

bool Asembler::handleGlobal(int* tokenCnt){
  //sym1, sym2, ... symN
  Token nextToken = tokens[(*tokenCnt)++];
  string name = nextToken.getText();
  Symbol* s;
  while(nextToken.getType() != TokenType::EOL) {
    s = symbolTable->findSymbol(name);
    if(s == nullptr) { //uvezen simbol
      s = symbolTable->addSymbol(name, 0, 0);
    }
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
          addRelocationABS(s, getCounter());
          val = 0;
        }
      }
  
    charr[0] = *((char*)&val+0); //little-endian
    charr[1] = *((char*)&val+1);
    charr[2] = *((char*)&val+2);
    charr[3] = *((char*)&val+3);
    write(charr, 4);
    nextToken = tokens[(*tokenCnt)++]; //pojedi zapetu
    if(nextToken.getType() == TokenType::EOL) break;
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

void Asembler::skipToNewLine(int* tokenCnt){
  Token nextToken = tokens[(*tokenCnt)++];
  while(nextToken.getType() != TokenType::EOL){
    nextToken = tokens[(*tokenCnt)++];
  }
}

void Asembler::writeInstruction(char* charr, char opcode, char a, char b, char c, uint16_t d){
  *(charr+0) = opcode;
  *(charr+1) = a << 4 | (b & 0x0F);
  *(charr+2) = (c << 4) | ((d >> 8) & 0x0F); //c, d1
  *(charr+3) = d & 0x00FF; //d2, d3
  write(charr, INSTRUCTION_SIZE);
}

void Asembler::handlePush(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::PUSH);
  string gpr = tokens[(*tokenCnt)++].getText();
  writeInstruction(charr, opcode, 14, 0, gprIndex(gpr), 0xFFFC);
}

void Asembler::handlePop(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::POP);
  string gpr = tokens[(*tokenCnt)++].getText();
  writeInstruction(charr, opcode, gprIndex(gpr), 14, 0, 0x0004);
}

void Asembler::handleRet(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::RET);
  writeInstruction(charr, opcode, 15, 14, 0, 0x0004);
}

bool Asembler::handleCall(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::CALL);
  writeInstruction(charr, opcode, 15, 0, 0, 0);
  return handleOperand(tokenCnt, -2); 
}

bool Asembler::handleJmp(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::JMP);
  writeInstruction(charr, opcode, 15, 0, 0, 0);
  return handleOperand(tokenCnt, -2);
}

bool Asembler::handleBeq(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::BEQ);
  string gpr1, gpr2;
  char b, c;
  gpr1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  gpr2 = tokens[(*tokenCnt)++].getText();
  b = gprIndex(gpr1);
  c = gprIndex(gpr2);
  (*tokenCnt)++; // pojedi zapetu
  writeInstruction(charr, opcode, 15, b, c, 0);
  return handleOperand(tokenCnt, -2);
}

bool Asembler::handleBne(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::BNE);
  string gpr1, gpr2;
  char b, c;
  gpr1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  gpr2 = tokens[(*tokenCnt)++].getText();
  b = gprIndex(gpr1);
  c = gprIndex(gpr2);
  (*tokenCnt)++; // pojedi zapetu
  writeInstruction(charr, opcode, 15, b, c, 0);
  return handleOperand(tokenCnt,  -2);
}

bool Asembler::handleBgt(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::BGT);
  string gpr1, gpr2;
  char b, c;
  gpr1 = tokens[(*tokenCnt)++].getText();
  (*tokenCnt)++; // pojedi zapetu
  gpr2 = tokens[(*tokenCnt)++].getText();
  b = gprIndex(gpr1);
  c = gprIndex(gpr2);
  (*tokenCnt)++; // pojedi zapetu
  writeInstruction(charr, opcode, 15, b, c, 0);
  return handleOperand(tokenCnt, -2);
}

void Asembler::handleXchg(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::XCHG);
  char b, c;
  handle2gpr(tokenCnt, &b, &c);
  writeInstruction(charr, opcode, 0, b, c, 0);
}

void Asembler::handleAdd(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::ADD);
  char a, b, c;
  handle2gpr(tokenCnt, &c, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, c, 0);
}

void Asembler::handleSub(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::SUB);
  char a, b, c;
  handle2gpr(tokenCnt, &c, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, c, 0);
}

void Asembler::handleMul(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::MUL);
  char a, b, c;
  handle2gpr(tokenCnt, &c, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, c, 0);
}

void Asembler::handleDiv(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::DIV);
  char a, b, c;
  handle2gpr(tokenCnt, &c, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, c, 0);
}

void Asembler::handleNot(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::SUB);
  char a, b;
  handle1gpr(tokenCnt, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, 0, 0);
}

void Asembler::handleAnd(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::AND);
  char a, b, c;
  handle2gpr(tokenCnt, &c, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, c, 0);
}

void Asembler::handleOr(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::OR);
  char a, b, c;
  handle2gpr(tokenCnt, &c, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, c, 0);
}

void Asembler::handleXor(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::XOR);
  char a, b, c;
  handle2gpr(tokenCnt, &c, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, c, 0);
}

void Asembler::handleShl(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::SHL);
  char a, b, c;
  handle2gpr(tokenCnt, &c, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, c, 0);  
}

void Asembler::handleShr(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::SHR);
  char a, b, c;
  handle2gpr(tokenCnt, &c, &b);
  a = b;
  writeInstruction(charr, opcode, a, b, c, 0);
}

bool Asembler::handleLdmemdir(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::LDMEMDIR);
  bool status = handleOperand(tokenCnt, 2);
  (*tokenCnt)++; // pojedi zapetu
  char a;
  handle1gpr(tokenCnt, &a);
  writeInstruction(charr, opcode, a, 15, 0, 0);
  //sada moramo da pristupimo memoriji na koju pokazuje podatak u a
  opcode = Token::getOPCodeBinary(TokenType::LDREGIND);
  char b = a;
  writeInstruction(charr, opcode, a, b, 0, 0);
  return status;
}

void Asembler::handleLdregdir(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::LDREGDIR);
  char a, b;
  handle2gpr(tokenCnt, &b, &a);
  writeInstruction(charr, opcode, a, b, 0, 0);
}

void Asembler::handleLdregind(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::LDREGIND);
  char a, b;
  handle1indir1gpr(tokenCnt, &b, &a);
  writeInstruction(charr, opcode, a, b, 0, 0);
}

bool Asembler::handleLdregindadd(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::LDREGINDADD);
  char a, b;
  uint16_t disp;
  bool status = handle1indirAddend1gpr(tokenCnt, &b, &a, &disp);
  writeInstruction(charr, opcode, a, b, 0, disp);
  return status;
}

bool Asembler::handleLdimmed(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::LDIMMED);
  string symbolOnly = tokens[*tokenCnt].getText(); //skini $
  symbolOnly = symbolOnly.substr(1);
  tokens[*tokenCnt].setText(symbolOnly);
  bool status = handleOperand(tokenCnt, 2);
  (*tokenCnt)++; // pojedi zapetu
  char a;
  handle1gpr(tokenCnt, &a);
  writeInstruction(charr, opcode, a, 15, 0, 0);
  return status;
}

bool Asembler::handleStmemdir(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::STMEMDIR);
  char c;
  handle1gpr(tokenCnt, &c);
  (*tokenCnt)++; // pojedi zapetu
  bool status = handleOperand(tokenCnt, 2);
  writeInstruction(charr, opcode, 0, 15, c, 0);
  return status;
}

void Asembler::handleStregdir(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::STREGDIR);
  char a, b;
  handle2gpr(tokenCnt, &b, &a);
  writeInstruction(charr, opcode, a, b, 0, 0);
}

void Asembler::handleStregind(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::STREGIND);
  char b, c;
  handle1gpr1indir(tokenCnt, &c, &b);
  writeInstruction(charr, opcode, 0, b, c, 0);
}

bool Asembler::handleStregindadd(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::STREGINDADD);
  char b, c;
  uint16_t disp;
  bool status = handle1gpr1indirAddend(tokenCnt, &c, &b, &disp);
  writeInstruction(charr, opcode, 0, b, c, disp);
  return status;
}


void Asembler::handleCsrrd(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::CSRRD);
  char a, b;
  handle1csr1gpr(tokenCnt, &b, &a);
  writeInstruction(charr, opcode, a, b, 0, 0);
}

void Asembler::handleCsrwr(int* tokenCnt, char* charr){
  char opcode = Token::getOPCodeBinary(TokenType::CSRWR);
  char a, b;
  handle1gpr1csr(tokenCnt, &b, &a);
  writeInstruction(charr, opcode, a, b, 0, 0);
}
