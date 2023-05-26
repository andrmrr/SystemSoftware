#include "tokens.h"

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
    case TokenType::SYM_CONST: out << "SYM_CONST"; break;
    case TokenType::LABEL: out << "LABEL"; break;
    case TokenType::DEC: out << "DEC"; break;
    case TokenType::HEX: out << "HEX"; break;
    case TokenType::LIT_DEC: out << "LIT_DEC"; break;
    case TokenType::LIT_HEX: out << "LIT_HEX"; break;
    case TokenType::REG: out << "REG"; break;
    case TokenType::COMMA: out << "COMMA"; break;
    case TokenType::COLON: out << "COLON"; break;
    case TokenType::PLUS: out << "PLUS"; break;
    case TokenType::MINUS: out << "MINUS"; break;
    case TokenType::BR_OPEN: out << "BR_OPEN"; break;
    case TokenType::BR_CLOSE: out << "BR_CLOSE"; break;
    case TokenType::DBL_QUOTES: out << "DBL_QUOTES"; break;
    case TokenType::EOL: out << "EOL"; break;
    case TokenType::END_FILE: out << "END_FILE"; break;
    case TokenType::NONE: out << "NONE"; break;
  }

  return out;
}