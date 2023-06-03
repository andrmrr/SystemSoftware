#include "asembler.h"
using namespace std;

Asembler::Asembler(vector<Token> tl) {
  error = false;
  lcounter = 0;
  tokens = tl;
  currSection = nullptr;
  SymbolTable::initSymbolTable();
  symbolTable = SymbolTable::getInstance();
}

Asembler::Asembler(vector<Token> tl, string outf){
  outputFilePath = outf;
  error = false;
  lcounter = 0;
  tokens = tl;
  currSection = nullptr;
  SymbolTable::initSymbolTable();
  symbolTable = SymbolTable::getInstance();
}

Asembler::~Asembler(){
  symbolTable->deleteInstance();
  for(auto it = relTables.begin(); it != relTables.end(); it ++){
    delete (*it);
  }
  secTable->deleteInstance();
}

bool const Asembler::getError(){
  return error;
}

int const Asembler::getCounter(){
  return lcounter;
}

void Asembler::incCounter(int inc){
  lcounter += inc;
}

void Asembler::resetCounter(){
  lcounter = 0;
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
  int temp, opCodeIndex;
  string strTemp;

  while(tokenCnt < tokens.size()){
    token = tokens[tokenCnt++];

    // cout << "Token " << token.getType() << " " << token.getText() << endl;

    switch(token.getType()) {
      case TokenType::EOL:
        break;
      case TokenType::HALT:
        syntaxError = check0(&tokenCnt) ? false : true;
        incCounter(1); // opcode
        // cout << "halt dobar!" << endl;
        break;
      case TokenType::INT:
        syntaxError = check0(&tokenCnt) ? false : true;
        incCounter(1); // opcode
        // cout << "int dobar!" << endl;
        break;
      case TokenType::IRET:
        syntaxError = check0(&tokenCnt) ? false : true;
        incCounter(1); // opcode
        // cout << "iret dobar!" << endl;
        break;
      case TokenType::CALL: 
        syntaxError = check1boperand(&tokenCnt) ? false : true;
        incCounter(5); // opcode-op[0]-op[1]-op[2]-op[3]
        // cout << "call dobar!" << endl;
        break;
      case TokenType::RET:
        syntaxError = check0(&tokenCnt) ? false : true;
        incCounter(1); // opcode
        // cout << "ret dobar!" << endl;
        break;
      case TokenType::JMP: 
        syntaxError = check1boperand(&tokenCnt) ? false : true;
        incCounter(5);  // opcode-op[0]-op[1]-op[2]-op[3]
        // cout << "jmp dobar!" << endl;
        break;
      case TokenType::BEQ: 
        syntaxError = check2gpr1boperand(&tokenCnt) ? false : true;
        incCounter(6); // opcode-(gpr1gpr2)-op[0]-op[1]-op[2]-op[3]
        // cout << "beq dobar!" << endl;
        break;
      case TokenType::BNE:
        syntaxError = check2gpr1boperand(&tokenCnt) ? false : true;
        incCounter(6); // opcode-(gpr1gpr2)-op[0]-op[1]-op[2]-op[3]
        // cout << "bne dobar!" << endl;
        break;
      case TokenType::BGT:
        syntaxError = check2gpr1boperand(&tokenCnt) ? false : true;
        incCounter(6); // opcode-(gpr1gpr2)-op[0]-op[1]-op[2]-op[3]
        // cout << "bgt dobar!" << endl;
        break;
      case TokenType::PUSH:
        syntaxError = check1gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(0x0gpr)
        // cout << "push dobar!" << endl;
        break;
      case TokenType::POP:
        syntaxError = check1gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(0x0gpr)
        // cout << "pop dobar!" << endl;
        break;
      case TokenType::XCHG:
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "xchg dobar!" << endl;
        break;
      case TokenType::ADD:
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "add dobar!" << endl;
        break;
      case TokenType::SUB:
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "sub dobar!" << endl;
        break;
      case TokenType::MUL: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "mul dobar!" << endl;
        break;
      case TokenType::DIV: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "div dobar!" << endl;
        break;
      case TokenType::NOT: 
        syntaxError = check1gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(0x0gpr)
        // cout << "not dobar!" << endl;
        break;
      case TokenType::AND: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "and dobar!" << endl;
        break;
      case TokenType::OR: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "or dobar!" << endl;
        break;
      case TokenType::XOR: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "xor dobar!" << endl;
        break;
      case TokenType::SHL: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "shl dobar!" << endl;
        break;
      case TokenType::SHR: 
        syntaxError = check2gpr(&tokenCnt) ? false : true;
        incCounter(2); // opcode-(gprDgprS)
        // cout << "shr dobar!" << endl;
        break;
      case TokenType::LD: 
        opCodeIndex = tokenCnt-1;
        temp = check1doperand1gpr(&tokenCnt);
        syntaxError = (temp == -1);
        if(!syntaxError){
          if(temp == 0) {
            tokens[opCodeIndex].setType(TokenType::LDMEMDIR); // opcode-(0x0gprD)-op[0]-op[1]-op[2]-op[3]
            incCounter(6);
          } else if(temp == 1) {
            tokens[opCodeIndex].setType(TokenType::LDREGDIR); // opcode-(gprDgprS)
            incCounter(2);
          } else if(temp == 2) {
            tokens[opCodeIndex].setType(TokenType::LDREGIND); // opcode-(gprDgprS)
            incCounter(2);
          } else if(temp == 3) {
            tokens[opCodeIndex].setType(TokenType::LDREGINDADD); // opcode-(gprDgprS)-op[0]-op[1]
            incCounter(4);
          } else {
            tokens[opCodeIndex].setType(TokenType::LDIMMED); // opcode-(0x0gprD)-op[0]-op[1]-op[2]-op[3]
            incCounter(6);
          }
        }
        // cout << "ld dobar!" << endl;
        break;
      case TokenType::ST: 
        opCodeIndex = tokenCnt-1;
        temp = check1gpr1doperand(&tokenCnt);
        syntaxError = (temp == -1);
        if(!syntaxError){
          if(temp == 0) {
            tokens[opCodeIndex].setType(TokenType::STMEMDIR); // opcode-(0x0gprD)-op[0]-op[1]-op[2]-op[3]
            incCounter(6);
          } else if(temp == 1) {
            tokens[opCodeIndex].setType(TokenType::STREGDIR);  // opcode-(gprDgprS)
            incCounter(2);
          } else if(temp == 2) {
            tokens[opCodeIndex].setType(TokenType::STREGIND); // opcode-(gprDgprS)
            incCounter(2);
          } else {
            tokens[opCodeIndex].setType(TokenType::STREGINDADD); // opcode-(gprDgprS)-op[0]-op[1]
            incCounter(4);
          }
        }
        // cout << "st dobar!" << endl;
        break;
      case TokenType::CSRRD:  
        syntaxError = check1csr1gpr(&tokenCnt) ? false : true; // opcode-(gprDcsrS)
        incCounter(2);
        // cout << "csrrd dobar!" << endl;
        break;
      case TokenType::CSRWR: 
        syntaxError = check1gpr1csr(&tokenCnt) ? false : true; // opcode-(csrDgprS)
        incCounter(2);
        // cout << "csrwr dobar!" << endl;
        break;
      case TokenType::LABEL:
        strTemp = token.getText();
        strTemp = strTemp.substr(0, strTemp.size()-1);
        symbolTable->addSymbol(strTemp, currSection->getSection(), lcounter);
        token.setText(strTemp);
        break;
      case TokenType::GLOBAL: 
        syntaxError = checkSymbolList(tokenCnt) ? false : true;
        // cout << "global dobar!" << endl;
        if(!syntaxError){
          while((nextToken = tokens[tokenCnt++].getType()).getType() != TokenType::EOL);
        }
        break;
      case TokenType::EXTERN: 
        syntaxError = checkSymbolList(tokenCnt) ? false : true;
        // cout << "extern dobar!" << endl;
        if(!syntaxError){
          while(true){
            nextToken = tokens[tokenCnt++];
            Symbol *s = symbolTable->addSymbol(nextToken.getText(), 0, 0);
            s->setGlobal();
            nextToken = tokens[tokenCnt++];
            if(nextToken.getType() == TokenType::EOL) break;
          }
        }
        break;
      case TokenType::SECTION:
        syntaxError = checkSymbol(tokenCnt) ? false : true;
        // cout << "section dobar!" << endl;
        if(!syntaxError) {
          closeSection();
          nextToken = tokens[tokenCnt++];
          openSection(nextToken.getText());
        }
        break;
      case TokenType::WORD:
        // cout << "word dobar!" << endl;
        temp = checkSymbolOrLiteralList(&tokenCnt);
        syntaxError = (temp == -1);
        if(!syntaxError){
          incCounter(4*temp);
        }
        break;
      case TokenType::SKIP:
        // cout << "skip dobar!" << endl;
        temp = checkLiteral(&tokenCnt);
        syntaxError = (temp == -1);
        if(!syntaxError){
          incCounter(temp);
        }
        break;
      case TokenType::ASCII:
        // cout << "ascii dobar!" << endl;
        temp = checkString(&tokenCnt);
        syntaxError = (temp == -1);
        if(!syntaxError){
          incCounter(temp);
        }
        break;
      case TokenType::EQU:
        // cout << "equ dobar!" << endl;
        syntaxError = checkExpr(tokenCnt) ? false : true;
        if(!syntaxError){
          syntaxError = handleEqu(&tokenCnt) ? false : true;
        }
        break;
      case TokenType::END:
        // cout << "end dobar!" << endl;
        closeSection();
        syntaxError = check0(&tokenCnt) ? false : true;
        return;
      default: syntaxError = true; break;
    }

    if(syntaxError){
      //uradi nesto
      cout << "GRESKA PRI PARSIRANJU U PRVOM PROLAZU" << endl;
      error = true;
      break;
    }
  }
  closeSection();
  //solve TNS
  cout << "Reda radi" << endl;
}

bool Asembler::check0(int* tokenCnt){
  Token nextToken;
  if(*tokenCnt < tokens.size()) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::EOL){
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
            return true;
          } else return false;
        } else return false;
      } else return false;  
    } else return false;
  } else return false;
}

int Asembler::check1doperand1gpr(int* tokenCnt){
  int retVal;
  Token nextToken;
  if(*tokenCnt < tokens.size() - 3) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::SYM_CONST || nextToken.getType() == TokenType::LIT_DEC || nextToken.getType() == TokenType::LIT_HEX){
      retVal = 4; //immed
    } else if(nextToken.getType() == TokenType::DEC || nextToken.getType() == TokenType::HEX || nextToken.getType() == TokenType::IDENT){
      retVal = 0; //memdir
    } else if(nextToken.getType() == TokenType::GPR){
      retVal = 1; //regdir
    } else if((retVal = isRegIndir(tokenCnt)) == -1) {
      return -1;  
    }
    if(*tokenCnt < tokens.size() - 2){
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::COMMA) {
        nextToken = tokens[(*tokenCnt)++];
        if(nextToken.getType() == TokenType::GPR) {
          nextToken = tokens[(*tokenCnt)++];
          if(nextToken.getType() == TokenType::EOL) {
            return retVal;
          } else return -1;
        } else return -1;
      } else return -1;
    } else return -1;
  } else return -1;
}

int Asembler::check1gpr1doperand(int* tokenCnt){
  Token nextToken;
  int retVal;
  if(*tokenCnt < tokens.size() - 3) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::GPR) {
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::COMMA) {
        nextToken = tokens[(*tokenCnt)++];
        if(nextToken.getType() == TokenType::DEC || nextToken.getType() == TokenType::HEX || nextToken.getType() == TokenType::IDENT){
          retVal = 0; //memdir
        } else if(nextToken.getType() == TokenType::GPR){
          retVal = 1; //regdir
        } else if((retVal = isRegIndir(tokenCnt)) == -1) {
          return -1;
        } 
        if(*tokenCnt < tokens.size()){
          nextToken = tokens[(*tokenCnt)++];
          if(nextToken.getType() == TokenType::EOL) {
            return retVal;
          } else return -1;
        } else return -1;
      } else return -1;  
    } else return -1;
  } else return -1;
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
            return true;
          } else return false;
        } else return false;
      } else return false;  
    } else return false;
  } else return false;    
}

int Asembler::isRegIndir(int* tokenCnt){
  (*tokenCnt)--;
  Token nextToken;
  if(*tokenCnt < tokens.size() - 2) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::BR_OPEN) {
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::GPR) {
        nextToken = tokens[(*tokenCnt)++];
        if(nextToken.getType() == TokenType::BR_CLOSE) {
          return 2; // regindir
        } else if(nextToken.getType() == TokenType::PLUS) {
          if(*tokenCnt < tokens.size() - 1) {
            nextToken = tokens[(*tokenCnt)++];
            if(Token::isBranchOperand(nextToken.getType())) {
              nextToken = tokens[(*tokenCnt)++];
              if(nextToken.getType() == TokenType::BR_CLOSE) {
                return 3; //regindir + addend
              } else return -1;
            } else return -1;
          } else return -1;
        } else return -1;
      } else return -1;
    } else return -1;  
  } else return -1;  
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

int Asembler::checkSymbolOrLiteralList(int* tokenCnt){
  int cnt = 0;
  Token nextToken;
  if((*tokenCnt) < tokens.size()){
    nextToken = tokens[(*tokenCnt)++];
    if(Token::isBranchOperand(nextToken.getType())){
      cnt++;
      while(true){
        if((*tokenCnt) < tokens.size()){
          nextToken = tokens[(*tokenCnt)++];
          if(nextToken.getType() == TokenType::EOL) {
            return cnt;
          } else if(nextToken.getType() == TokenType::COMMA) {
            if((*tokenCnt) < tokens.size()){
              nextToken = tokens[(*tokenCnt)++];
              if(Token::isBranchOperand(nextToken.getType())){
                cnt++;
                continue;
              } else return -1;
            } else return -1;
          } else return -1;
        } else return -1;
      }
    } else return -1; 
  } else return -1; 
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

int Asembler::checkLiteral(int* tokenCnt){
  int val;
  Token nextToken;
  if((*tokenCnt) < tokens.size()-1) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::HEX || nextToken.getType() == TokenType::DEC){
      if(nextToken.getType() == TokenType::HEX){
        val = hexStringToInt(nextToken.getText());
      } else {
        val = atoi(nextToken.getText().c_str());
      }
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::EOL) {
        return val;
      } else return -1;
    } else return -1;
  } return -1;
}

int Asembler::checkString(int* tokenCnt){
  Token nextToken;
  int size;
  if((*tokenCnt) < tokens.size()-1) {
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::STRING){
      size = nextToken.getText().size();
      nextToken = tokens[(*tokenCnt)++];
      if(nextToken.getType() == TokenType::EOL) {
        return size;
      } else return -1;
    } else return -1;
  } return -1;
}

bool Asembler::checkExpr(int tokenCnt){
  //sym, sym/lit [+/- sym/lit]
  Token nextToken;
  if(tokenCnt < tokens.size() - 3) {
    nextToken = tokens[tokenCnt++];
    if(nextToken.getType() == TokenType::IDENT){
      nextToken = tokens[tokenCnt++];
      if(nextToken.getType() == TokenType::COMMA){
        nextToken = tokens[tokenCnt++];
        if(Token::isBranchOperand(nextToken.getType())){
          nextToken = tokens[tokenCnt++];
          if(nextToken.getType() == TokenType::EOL) {
            return true;
          } else if(nextToken.getType() == TokenType::PLUS || nextToken.getType() == TokenType::MINUS) {
            if(tokenCnt < tokens.size()-1){
                nextToken = tokens[tokenCnt++];
                if(Token::isBranchOperand(nextToken.getType())){
                  nextToken = tokens[tokenCnt++];
                  if(nextToken.getType() == TokenType::EOL) {
                    return true;
                  } else return false;
                } else return false;
            } else return false;
          } else return false;       
        } else return false;  
      } else return false;
    } else return false;
  } else return false;
}

bool Asembler::handleEqu(int* tokenCnt){
  string sym;
  int val;
  Token nextToken = tokens[(*tokenCnt)++];
  sym = nextToken.getText();
  tokens[(*tokenCnt)++]; // pojedi zapetu

  nextToken = tokens[(*tokenCnt)++];
  if(nextToken.getType() == TokenType::HEX){
    val = hexStringToInt(nextToken.getText());
  } else if(nextToken.getType() == TokenType::DEC){
    val = atoi(nextToken.getText().c_str());
  } else { // symbol
    Symbol* s = symbolTable->findSymbol(nextToken.getText());
    if(s == nullptr || !s->isAbsolute()) {
      cout << "Nedozvoljen operand EQU instrukcije" << endl;
      return false;
    } else {
      val = s->getValue();
    }
  }

  nextToken = tokens[(*tokenCnt)++];
  if(nextToken.getType() == TokenType::PLUS){
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::HEX){
      val += hexStringToInt(nextToken.getText());
    } else if(nextToken.getType() == TokenType::DEC){
      val += atoi(nextToken.getText().c_str());
    } else { // symbol
      Symbol* s = symbolTable->findSymbol(nextToken.getText());
      if(s == nullptr || !s->isAbsolute()) {
        cout << "Nedozvoljen operand EQU instrukcije" << endl;
        return false;
      } else {
        val += s->getValue();
      }
    }  
  } else if(nextToken.getType() == TokenType::MINUS){
    nextToken = tokens[(*tokenCnt)++];
    if(nextToken.getType() == TokenType::HEX){
      val -= hexStringToInt(nextToken.getText());
    } else if(nextToken.getType() == TokenType::DEC){
      val -= atoi(nextToken.getText().c_str());
    } else { // symbol
      Symbol* s = symbolTable->findSymbol(nextToken.getText());
      if(s == nullptr || !s->isAbsolute()) {
        cout << "Nedozvoljen operand EQU instrukcije" << endl;
        return false;
      } else {
        val -= s->getValue();
      }
    }
  }

  symbolTable->addAbsolute(sym, val);
  return true;
}

int Asembler::hexStringToInt(string hex){
  int val = 0;
  char current;
  for(int i = 2; i < hex.size(); i++){
    current = hex[i];
    val *= 16;
    switch(current){
      case '0':
        val += 0;
        break;
      case '1':
        val += 1;
        break;
      case '2':
        val += 2;
        break;
      case '3':
        val += 3;
        break;
      case '4':
        val += 4;
        break;
      case '5':
        val += 5;
        break;
      case '6':
        val += 6;
        break;
      case '7':
        val += 7;
        break;
      case '8':
        val += 8;
        break;
      case '9':
        val += 9;
        break;
      case 'a':
      case 'A':
        val += 10;
        break;
      case 'b':
      case 'B':
        val += 11;
        break;
      case 'c':
      case 'C':
        val += 12;
        break;
      case 'd':
      case 'D':
        val += 13;
        break;
      case 'e':
      case 'E':
        val += 14;
        break;
      case 'f':
      case 'F':
        val += 15;
        break;  
    }
  }
  return val;
}