#pragma once

#include <string>
#include <ostream>

using namespace std;

enum class TokenType {
  HALT, INT, IRET, CALL, RET, JMP, BEQ, BNE, BGT, PUSH, POP, XCHG, ADD, SUB, MUL, DIV, NOT, AND, OR, XOR, SHL, SHR, LD, ST, CSRRD, CSRWR,
  GLOBAL, EXTERN, SECTION, WORD, SKIP, ASCII, EQU, END,
  IDENT, SYM_CONST, LABEL,
  DEC, HEX, LIT_DEC, LIT_HEX,
  REG,
  COMMA, COLON, PLUS, MINUS, BR_OPEN, BR_CLOSE, DBL_QUOTES,
  EOL, END_FILE, NONE, ERROR
};

ostream& operator<<(std::ostream& out, const TokenType tokenType);

class Token{
  TokenType type;
  string text;

public:
  Token(TokenType tt = TokenType::END_FILE, string tx = "") : type(tt), text(tx) {}
  TokenType getType() const { return type; }
  string getText() const { return text; }
  static bool isBranchOperand(TokenType tokenType);
};