#include "tokens.h"

void Token::setType(TokenType tt){
  this->type = tt;
}

void Token::setText(string s){
  this->text = s;
}

bool Token::isBranchOperand(TokenType tokenType){
  if(tokenType == TokenType::DEC || tokenType == TokenType::HEX || tokenType == TokenType::IDENT)
    return true;
  return false;
}

bool Token::isCsr(TokenType tokenType){
  if(tokenType == TokenType::STATUS || tokenType == TokenType::HANDLER || tokenType == TokenType::CAUSE)
    return true;
  return false;
}



char Token::getOPCodeBinary(TokenType tokenType){
  switch(tokenType) {
    case TokenType::HALT: return 0x00;
    case TokenType::INT: return 0x10;
    case TokenType::IRET: return 0x11;
    case TokenType::CALL: return 0x21;
    case TokenType::JMP: return 0x38;
    case TokenType::BEQ: return 0x39;
    case TokenType::BNE: return 0x3a;
    case TokenType::BGT: return 0x3b;
    case TokenType::XCHG: return 0x40;
    case TokenType::ADD: return 0x50;
    case TokenType::SUB: return 0x51;
    case TokenType::MUL: return 0x52;
    case TokenType::DIV: return 0x53;
    case TokenType::NOT: return 0x60;
    case TokenType::AND: return 0x61;
    case TokenType::OR: return 0x62;
    case TokenType::XOR: return 0x63;
    case TokenType::SHL: return 0x70;
    case TokenType::SHR: return 0x71;
    case TokenType::PUSH: return 0x81;
    case TokenType::LDMEMDIR: return 0x92;
    case TokenType::LDREGDIR: return 0x91;
    case TokenType::LDREGIND: return 0x92;
    case TokenType::LDREGINDADD: return 0x92;
    case TokenType::LDIMMED: return 0x92;
    case TokenType::STMEMDIR: return 0x82;
    case TokenType::STREGDIR: return 0x91;
    case TokenType::STREGIND: return 0x80;
    case TokenType::STREGINDADD: return 0x80;
    case TokenType::POP: return 0x93;
    case TokenType::RET: return 0x93;
    case TokenType::CSRRD: return 0x90;
    case TokenType::CSRWR: return 0x94;
    default: return 0xff;
  }
}

ostream& operator<<(std::ostream& out, const TokenType tokenType) {
  switch(tokenType) {
    case TokenType::HALT: out << "HALT"; break;
    case TokenType::INT: out << "INT"; break;
    case TokenType::IRET: out << "IRET"; break;
    case TokenType::CALL: out << "CALL"; break;
    case TokenType::RET: out << "RET"; break;
    case TokenType::JMP: out << "JMP"; break;
    case TokenType::BEQ: out << "BEQ"; break;
    case TokenType::BNE: out << "BNE"; break;
    case TokenType::BGT: out << "BGT"; break;
    case TokenType::PUSH: out << "PUSH"; break;
    case TokenType::POP: out << "POP"; break;
    case TokenType::XCHG: out << "XCHG"; break;
    case TokenType::ADD: out << "ADD"; break;
    case TokenType::SUB: out << "SUB"; break;
    case TokenType::MUL: out << "MUL"; break;
    case TokenType::DIV: out << "DIV"; break;
    case TokenType::NOT: out << "NOT"; break;
    case TokenType::AND: out << "AND"; break;
    case TokenType::OR: out << "OR"; break;
    case TokenType::XOR: out << "XOR"; break;
    case TokenType::SHL: out << "SHL"; break;
    case TokenType::SHR: out << "SHR"; break;
    case TokenType::LD: out << "LD"; break;
    case TokenType::ST: out << "ST"; break;
    case TokenType::LDMEMDIR: out << "LDMEMDIR"; break;
    case TokenType::LDREGDIR: out << "LDREGDIR"; break;
    case TokenType::LDREGIND: out << "LDREGIND"; break;
    case TokenType::LDREGINDADD: out << "LDREGINDADD"; break;
    case TokenType::LDIMMED: out << "LDIMMED"; break;
    case TokenType::STMEMDIR: out << "STMEMDIR"; break;
    case TokenType::STREGDIR: out << "STREGDIR"; break;
    case TokenType::STREGIND: out << "STREGIND"; break;
    case TokenType::STREGINDADD: out << "STREGINDADD"; break;
    case TokenType::CSRRD: out << "CSRRD"; break;
    case TokenType::CSRWR: out << "CSRWR"; break;
    case TokenType::GLOBAL: out << "GLOBAL"; break;
    case TokenType::EXTERN: out << "EXTERN"; break;
    case TokenType::SECTION: out << "SECTION"; break;
    case TokenType::WORD: out << "WORD"; break;
    case TokenType::SKIP: out << "SKIP"; break;
    case TokenType::ASCII: out << "ASCII"; break;
    case TokenType::EQU: out << "EQU"; break;
    case TokenType::END: out << "END"; break;
    case TokenType::IDENT: out << "IDENT"; break;
    case TokenType::STRING: out << "STRING"; break;
    case TokenType::SYM_CONST: out << "SYM_CONST"; break;
    case TokenType::LABEL: out << "LABEL"; break;
    case TokenType::DEC: out << "DEC"; break;
    case TokenType::HEX: out << "HEX"; break;
    case TokenType::LIT_DEC: out << "LIT_DEC"; break;
    case TokenType::LIT_HEX: out << "LIT_HEX"; break;
    case TokenType::GPR: out << "GPR"; break;
    case TokenType::STATUS: out << "STATUS"; break;
    case TokenType::HANDLER: out << "HANDLER"; break;
    case TokenType::CAUSE: out << "CAUSE"; break;
    case TokenType::COMMA: out << "COMMA"; break;
    case TokenType::COLON: out << "COLON"; break;
    case TokenType::PLUS: out << "PLUS"; break;
    case TokenType::MINUS: out << "MINUS"; break;
    case TokenType::BR_OPEN: out << "BR_OPEN"; break;
    case TokenType::BR_CLOSE: out << "BR_CLOSE"; break;
    case TokenType::EOL: out << "EOL"; break;
    case TokenType::END_FILE: out << "END_FILE"; break;
    case TokenType::NONE: out << "NONE"; break;
  }

  return out;
}