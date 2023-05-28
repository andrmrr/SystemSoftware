#pragma once

#include <vector>
#include "tokens.h"
#include "symbolTable.h"


#define INSTRUCTION_SIZE 4

class Asembler {

  vector<Token> tokens;
  int lcounter;

  Symbol* currSection;
  SymbolTable* symbolTable;

public:
  Asembler(vector<Token> tlist);

  //focal methods
  void firstPass();
  void secondPass();

  //getters and setters
  void incCounter(int inc);
  int const getCounter();

  //utility methods
  bool check0(int* tokenCnt); // check instruction with no arguments
  bool check1boperand(int* tokenCnt); // check instruction with 1 branch operand
  bool check2gpr1boperand(int* tokenCnt); // check instruction with 2 gprs and 1 branch operand
  bool check1gpr(int* tokenCnt); // check instruction with 1 gpr
  bool check2gpr(int* tokenCnt); // check instruction with 2 gprs
  bool check1doperand1gpr(int* tokenCnt); // check instruction with 1 data operand and 1 gpr
  bool check1gpr1doperand(int* tokenCnt); // check instruction with 1 gpr and 1 data operand
  bool check1csr1gpr(int* tokenCnt); // check instruction with 1 csr and 1 gpr
  bool check1gpr1csr(int* tokenCnt); // check instruction with 1 gpr and 1 csr
  bool isRegIndir(int* tokenCnt); // check if the following tokens form the syntax of reg. indir. (addend) address

  bool checkSymbolList(int tokenCnt);
  bool checkSymbolOrLiteralList(int* tokenCnt);
  bool checkSymbol(int tokenCnt);
  bool checkLiteral(int* tokenCnt);
  bool checkString(int* tokenCnt);
  bool checkExpr(int* tokenCnt);
  
  void openSection(string name);
  void closeSection();


};