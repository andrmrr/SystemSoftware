#pragma once

#include <string>
#include <ostream>

using namespace std;

enum class TokenType {
  HALT, INT, IRET, CALL, RET, JMP, BEQ, BNE, BGT, PUSH, POP, XCHG, ADD, SUB, MUL, DIV, NOT, AND, OR, XOR, SHL, SHR, LD, ST, CSRRD, CSRWR,
  GLOBAL, EXTERN, SECTION, WORD, SKIP, ASCII, EQU, END,
  LDMEMDIR, LDREGDIR, LDREGIND, LDREGINDADD, LDIMMED, STMEMDIR, STREGDIR, STREGIND, STREGINDADD,
  IDENT, SYM_CONST, LABEL, STRING,
  DEC, HEX, LIT_DEC, LIT_HEX,
  GPR, STATUS, HANDLER, CAUSE,
  COMMA, COLON, PLUS, MINUS, BR_OPEN, BR_CLOSE,
  EOL, END_FILE, NONE, ERROR
};

ostream& operator<<(std::ostream& out, const TokenType tokenType);

class Token{
  TokenType type;
  string text;

public:
  Token(TokenType tt = TokenType::END_FILE, string tx = "") : type(tt), text(tx) {}
  TokenType getType() const { return type; }
  void setType(TokenType tt);
  string getText() const { return text; }
  void setText(string s);

  static bool isBranchOperand(TokenType tokenType);
  static bool isCsr(TokenType tokenType);
  static char getOPCodeBinary(TokenType tokenType);
};