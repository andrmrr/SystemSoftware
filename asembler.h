#pragma once

#include <vector>
#include "tokens.h"

#define INSTRUCTION_SIZE 4

class Asembler {

  vector<Token> tokens;
  int lcounter;

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
  bool check2gpr1boperand(); // check instruction with 2 gprs and 1 branch operand
  bool check1gpr(); // check instruction with 1 gpr
  bool check2gpr(); // check instruction with 2 gprs
  bool check1doperand1gpr(); // check instruction with 1 data operand and 1 gpr
  bool check1gpr1doperand(); // check instruction with 1 gpr and 1 data operand
  bool check1csr1gpr(); // check instruction with 1 csr and 1 gpr
  bool check1gpr1csr(); // check instruction with 1 gpr and 1 csr


};