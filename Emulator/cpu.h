#pragma once

#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

#define SP 14
#define PC 15
#define STATUS 0
#define HANDLER 1
#define CAUSE 2

typedef uint32_t Word;
typedef struct instruction{
  char a, b, c;
  int16_t d;
  instruction(char a, char b, char c, int16_t d) : a(a), b(b), c(c), d(d) {}
} Instruction;


class CPU {
  typedef enum { running, finished, no_data, bad_data, bad_op } State;
  map<uint32_t, Word> memory;
  Word gpr[16];
  Word csr[3];
  State state;
  //private methods for memory manipulation

  void writeWord(uint32_t addr, Word w);
  Word readWord(uint32_t addr);

public:
  CPU();
  ~CPU();
  void reset();
  void loadMemory(string ifName);
  void run();
  void print();

  //utility functions
  vector<string> split(const string line, char delim);
  uint32_t hexStringToUInt32(string hex, bool prefix);

  //instruction executions
  void execCall(Instruction instr);
  void execJmp(Instruction instr);
  void execBeq(Instruction instr);
  void execBne(Instruction instr);
  void execBgt(Instruction instr);

  void execXchg(Instruction instr);
  void execAdd(Instruction instr);
  void execSub(Instruction instr);
  void execMul(Instruction instr);
  void execDiv(Instruction instr);

  void execNot(Instruction instr);
  void execAnd(Instruction instr);
  void execOr(Instruction instr);
  void execXor(Instruction instr);
  void execShl(Instruction instr);
  void execShr(Instruction instr);

  void execPush(Instruction instr);
  void execPop(Instruction instr);
  void execStmem(Instruction instr);
  void execStmemmem(Instruction instr);
  void execRegReg(Instruction instr);
  void execLdmem(Instruction instr);
  void execCsrrd(Instruction instr);
  void execCsrwr(Instruction instr);

  

};