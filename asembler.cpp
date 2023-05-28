#include "asembler.h"
#include <iostream>
using namespace std;

Asembler::Asembler(vector<Token> tl) {
  lcounter = 0;
  tokens = tl;
  currSection = nullptr;
  SymbolTable::initSymbolTable();
  symbolTable = SymbolTable::getInstace();
}

int const Asembler::getCounter(){
  return lcounter;
}

void Asembler::incCounter(int inc){
  lcounter += inc;
}

void Asembler::openSection(string name){
  currSection = symbolTable->addSection(name);
}

void Asembler::closeSection(){
  if(currSection != nullptr) {
    currSection->setSize(lcounter);
  }
  lcounter = 0;
}

void Asembler::firstPass(){
  bool syntaxError = false;
  int tokenCnt = 0;
  Token token, nextToken;

  while(tokenCnt < tokens.size()){
    token = tokens[tokenCnt++];

    // cout << "Token " << token.getType() << " " << token.getText() << endl;

    switch(token.getType()) {
      case TokenType::EOL:
        break;
      case TokenType::HALT:
        syntaxError = check0(&tokenCnt) ? false : true;
        cout << "halt dobar!" << endl;
        break;
      case TokenType::INT:
        syntaxError = check0(&tokenCnt) ? false : true;
        cout << "int dobar!" << endl;
        break;
      case TokenType::IRET:
        syntaxError = check0(&tokenCnt) ? false : true;
        cout << "iret dobar!" << endl;
        break;
      case TokenType::CALL: 
        syntaxError = check1boperand(&tokenCnt) ? false : true;
        cout << "call dobar!" << endl;
        break;
      case TokenType::RET:
        syntaxError = check0(&tokenCnt) ? false : true;
        cout << "ret dobar!" << endl;
        break;
      case TokenType::JMP: 
        syntaxError = check1boperand(&tokenCnt) ? false : true;
        cout << "jmp dobar!" << endl;
        break;
      case TokenType::BEQ: 
        syntaxError = check2gpr1boperand(&tokenCnt) ? false : true;
        cout << "beq dobar!" << endl;
        break;
      case TokenType::BNE:
        syntaxError = check2gpr1boperand(&tokenCnt) ? false : true;
        cout << "bne dobar!" << endl;
        break;
      case TokenType::BGT:
        syntaxError = check2gpr1boperand(&tokenCnt) ? false : true;
        cout << "bgt dobar!" << endl;
        break;
      case TokenType::PUSH:
        syntaxError = check1gpr(&tokenCnt) ? false : true;
        cout << "push dobar!" << endl;
        break;
      case TokenType::POP:
        syntaxError = check1gpr(&tokenCnt) ? false : true;
        cout << "pop dobar!" << endl;
        break;
      case TokenType::XCHG:
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "add dobar!" << endl;
        break;
      case TokenType::ADD:
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "add dobar!" << endl;
        break;
      case TokenType::SUB:
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "sub dobar!" << endl;
        break;
      case TokenType::MUL: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "mul dobar!" << endl;
        break;
      case TokenType::DIV: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "div dobar!" << endl;
        break;
      case TokenType::NOT: 
        syntaxError = check1gpr(&tokenCnt) ? false : true;
        cout << "not dobar!" << endl;
        break;
      case TokenType::AND: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "and dobar!" << endl;
        break;
      case TokenType::OR: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "or dobar!" << endl;
        break;
      case TokenType::XOR: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "xor dobar!" << endl;
        break;
      case TokenType::SHL: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "shl dobar!" << endl;
        break;
      case TokenType::SHR: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        cout << "shr dobar!" << endl;
        break;
      case TokenType::LD: 
        syntaxError = check1doperand1gpr(&tokenCnt) ? false : true;
        cout << "ld dobar!" << endl;
        break;
      case TokenType::ST: 
        syntaxError = check1gpr1doperand(&tokenCnt) ? false : true;
        cout << "st dobar!" << endl;
        break;
      case TokenType::CSRRD:  
        syntaxError = check1csr1gpr(&tokenCnt) ? false : true;
        cout << "csrrd dobar!" << endl;
        break;
      case TokenType::CSRWR: 
        syntaxError = check1gpr1csr(&tokenCnt) ? false : true;
        cout << "csrwr dobar!" << endl;
        break;
      case TokenType::GLOBAL: 
        syntaxError = checkSymbolList(tokenCnt) ? false : true;
        cout << "global dobar!" << endl;
        if(!syntaxError){
          Token nextToken;
          while((nextToken = tokens[tokenCnt++].getType()).getType() != TokenType::EOL);
        }
        break;
      case TokenType::EXTERN: 
        syntaxError = checkSymbolList(tokenCnt) ? false : true;
        cout << "extern dobar!" << endl;
        if(!syntaxError){
          Token nextToken;
          while(true){
            nextToken = tokens[tokenCnt++];
            symbolTable->addSymbol(nextToken.getText(), 0);
            nextToken = tokens[tokenCnt++];
            if(nextToken.getType() == TokenType::EOL) break;
          }
        }
        break;
      case TokenType::SECTION:
        syntaxError = checkSymbol(tokenCnt) ? false : true;
        cout << "section dobar!" << endl;
        if(!syntaxError) {
          closeSection();
          Token nextToken;
          nextToken = tokens[tokenCnt++];
          openSection(nextToken.getText());
        }
        break;
      case TokenType::WORD:
        syntaxError = checkSymbolOrLiteralList(&tokenCnt) ? false : true;
        cout << "word dobar!" << endl;
        break;
      case TokenType::SKIP:
        cout << "skip dobar!" << endl;
        syntaxError = checkLiteral(&tokenCnt) ? false : true;
        break;
      case TokenType::ASCII:
        cout << "ascii dobar!" << endl;
        syntaxError = checkString(&tokenCnt) ? false : true;
        break;
      case TokenType::EQU:
        cout << "equ dobar!" << endl;
        syntaxError = checkExpr(&tokenCnt) ? false : true;
        break;
      case TokenType::END:
        cout << "end dobar!" << endl;
        syntaxError = check0(&tokenCnt) ? false : true;
        return;
        break;
      default: syntaxError = true; break;
    }

    if(syntaxError){
      //uradi nesto
      cout << "GRESKA PRI PARSIRANJU" << endl;
      break;
    }
  }
}

bool Asembler::check0(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size()) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::EOL){
      incCounter(INSTRUCTION_SIZE);
      return true;
    } else return false;
  } else return false;
}

bool Asembler::check1boperand(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size()-1) {
    nextToken = tokens[(*tokenCnt)++];
    if(Token::isBranchOperand(nextToken.getType())){
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::EOL){
        incCounter(INSTRUCTION_SIZE);
        return true;
      } else return false;
    } else return false;
  } else return false;
}

bool Asembler::check2gpr1boperand(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size() - 5) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::GPR) {
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::COMMA) {
        nextToken = tokens[(*tokenCnt)++];
        if(nextToken.getType() == TokenType::GPR) {
          nextToken = tokens[(*tokenCnt)++];
          if(nextToken.getType() == TokenType::COMMA) {
            nextToken = tokens[(*tokenCnt)++];
            if(Token::isBranchOperand(nextToken.getType())){
              nextToken = tokens[(*tokenCnt)++];
              if(nextToken.getType() == TokenType::EOL) {
                incCounter(INSTRUCTION_SIZE);
                return true;
              } else return false;
            } else return false;
          } else return false;
        } else return false;
      } else return false;  
    } else return false;
  } else return false;
}

bool Asembler::check1gpr(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size()-1){
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::GPR){
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::EOL){
        incCounter(INSTRUCTION_SIZE);
        return true;
      } else return false;
    } else return false;
  } else return false;
}

bool Asembler::check2gpr(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size() - 3) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::GPR) {
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::COMMA) {
        nextToken = tokens[(*tokenCnt)++];
        if(nextToken.getType() == TokenType::GPR) {
          nextToken = tokens[(*tokenCnt)++];
          if(nextToken.getType() == TokenType::EOL) {
            incCounter(INSTRUCTION_SIZE);
            return true;
          } else return false;
        } else return false;
      } else return false;  
    } else return false;
  } else return false;
}

bool Asembler::check1doperand1gpr(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size() - 3) {
    nextToken = tokens[(*tokenCnt)++];
    if(Token::isSimpleDataOperand(nextToken.getType()) || isRegIndir(tokenCnt)) {
      if(*tokenCnt < tokens.size() - 2){
        nextToken = tokens[(*tokenCnt)++];
        if(nextToken.getType() == TokenType::COMMA) {
          nextToken = tokens[(*tokenCnt)++];
          if(nextToken.getType() == TokenType::GPR) {
            nextToken = tokens[(*tokenCnt)++];
            if(nextToken.getType() == TokenType::EOL) {
              incCounter(INSTRUCTION_SIZE);
              return true;
            } else return false;
          } else return false;
        } else return false;
      } else return false;  
    } else return false;
  } else return false;
}

bool Asembler::check1gpr1doperand(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size() - 3) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::GPR) {
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::COMMA) {
        nextToken = tokens[(*tokenCnt)++];
        if(Token::isSimpleDataOperand(nextToken.getType()) || isRegIndir(tokenCnt)) {
          if(*tokenCnt < tokens.size()){
            nextToken = tokens[(*tokenCnt)++];
            if(nextToken.getType() == TokenType::EOL) {
              incCounter(INSTRUCTION_SIZE);
              return true;
            } else return false;
          } else return false;
        } else return false;
      } else return false;  
    } else return false;
  } else return false;
}

bool Asembler::check1csr1gpr(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size() - 3) {
    nextToken = tokens[(*tokenCnt)++];
    if(Token::isCsr(nextToken.getType())) {
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::COMMA) {
        nextToken = tokens[(*tokenCnt)++];
        if(nextToken.getType() == TokenType::GPR) {
          nextToken = tokens[(*tokenCnt)++];
          if(nextToken.getType() == TokenType::EOL) {
            incCounter(INSTRUCTION_SIZE);
            return true;
          } else return false;
        } else return false;
      } else return false;  
    } else return false;
  } else return false;
}

bool Asembler::check1gpr1csr(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size() - 3) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::GPR) {
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::COMMA) {
        nextToken = tokens[(*tokenCnt)++];
        if(Token::isCsr(nextToken.getType())) {
          nextToken = tokens[(*tokenCnt)++];
          if(nextToken.getType() == TokenType::EOL) {
            incCounter(INSTRUCTION_SIZE);
            return true;
          } else return false;
        } else return false;
      } else return false;  
    } else return false;
  } else return false;    
}

bool Asembler::isRegIndir(int* tokenCnt){
  (*tokenCnt)--;
  Token nextToken;
  if(*tokenCnt < tokens.size() - 2) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::BR_OPEN) {
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::GPR) {
        nextToken = tokens[(*tokenCnt)++];
        if(nextToken.getType() == TokenType::BR_CLOSE) {
          return true;
        } else if(nextToken.getType() == TokenType::PLUS) {
          if(*tokenCnt < tokens.size() - 1) {
            nextToken = tokens[(*tokenCnt)++];
            if(Token::isBranchOperand(nextToken.getType())) {
              nextToken = tokens[(*tokenCnt)++];
              if(nextToken.getType() == TokenType::BR_CLOSE) {
                return true;
              } else return false;
            } else return false;
          } else return false;
        } else return false;
      } else return false;
    } else return false;  
  } else return false;  
}

bool Asembler::checkSymbolList(int tokenCnt){
  Token nextToken;
  if(tokenCnt < tokens.size()){
    nextToken = tokens[tokenCnt++];
    if(nextToken.getType() == TokenType::IDENT){
      while(true){
        if(tokenCnt < tokens.size()){
          nextToken = tokens[tokenCnt++];
          if(nextToken.getType() == TokenType::EOL) {
            return true;
          } else if(nextToken.getType() == TokenType::COMMA) {
            if(tokenCnt < tokens.size()){
              nextToken = tokens[tokenCnt++];
              if(nextToken.getType() == TokenType::IDENT) {
                continue;
              } else return false;
            } else return false;
          } else return false;
        } else return false;
      }
    } else return false;
  } else return false;
}

bool Asembler::checkSymbolOrLiteralList(int* tokenCnt){
  Token nextToken;
  if((*tokenCnt) < tokens.size()){
    nextToken = tokens[(*tokenCnt)++];
    if(Token::isBranchOperand(nextToken.getType())){
      while(true){
        if((*tokenCnt) < tokens.size()){
          nextToken = tokens[(*tokenCnt)++];
          if(nextToken.getType() == TokenType::EOL) {
            return true;
          } else if(nextToken.getType() == TokenType::COMMA) {
            if((*tokenCnt) < tokens.size()){
              nextToken = tokens[(*tokenCnt)++];
              if(Token::isBranchOperand(nextToken.getType())){
                continue;
              } else return false;
            } else return false;
          } else return false;
        } else return false;
      }
    } else return false; 
  } else return false; 
}

bool Asembler::checkSymbol(int tokenCnt){
  Token nextToken;
  if(tokenCnt < tokens.size()-1) {
    nextToken = tokens[tokenCnt++];
    if(nextToken.getType() == TokenType::IDENT){
      nextToken = tokens[tokenCnt++];
      if(nextToken.getType() == TokenType::EOL) {
        return true;
      } else return false;
    } else return false;
  } return false;
}

bool Asembler::checkLiteral(int* tokenCnt){
  Token nextToken;
  if((*tokenCnt) < tokens.size()-1) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::HEX || nextToken.getType() == TokenType::DEC){
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::EOL) {
        return true;
      } else return false;
    } else return false;
  } return false;
}

bool Asembler::checkString(int* tokenCnt){
  Token nextToken;
  if((*tokenCnt) < tokens.size()-1) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::STRING){
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::EOL) {
        return true;
      } else return false;
    } else return false;
  } return false;
}

bool Asembler::checkExpr(int* tokenCnt){
  //sym, expr
  Token nextToken;
  if((*tokenCnt) < tokens.size() - 3) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::IDENT){
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::COMMA){
        nextToken = tokens[(*tokenCnt)++];
        if(Token::isBranchOperand(nextToken.getType())){
          while(true){
            if((*tokenCnt) < tokens.size()){
              nextToken = tokens[(*tokenCnt)++];
              if(nextToken.getType() == TokenType::EOL) {
                return true;
              } else if(nextToken.getType() == TokenType::PLUS || nextToken.getType() == TokenType::MINUS) {
                if((*tokenCnt) < tokens.size()){
                  nextToken = tokens[(*tokenCnt)++];
                  if(Token::isBranchOperand(nextToken.getType())){
                    continue;
                  } else return false;
                } else return false;
              } else return false;
            } else return false;
          }
        } else return false;  
      } else return false;
    } else return false;
  } else return false;
}