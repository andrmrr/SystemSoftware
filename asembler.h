#pragma once

#include <vector>
#include "tokens.h"
#include "symbolTable.h"
#include "relocationTable.h"
#include "sectionTable.h"


#define INSTRUCTION_SIZE 4

class Asembler {

  vector<Token> tokens;
  int lcounter;
  int line;

  Symbol* currSection;
  SymbolTable* symbolTable;
  vector<RelocationTable*> relTables;
  SectionTable* secTable;
  bool error;

public:
  Asembler(vector<Token> tlist);
  ~Asembler();

  void secondPassInit();

  //focal methods
  void firstPass();
  void secondPass();

  //getters and setters
  bool const getError();
  void incCounter(int inc);
  void resetCounter();
  int const getCounter();

  //syntax check methods PASS1
  bool check0(int* tokenCnt); // check instruction with no arguments
  bool check1boperand(int* tokenCnt); // check instruction with 1 branch operand
  bool check2gpr1boperand(int* tokenCnt); // check instruction with 2 gprs and 1 branch operand
  bool check1gpr(int* tokenCnt); // check instruction with 1 gpr
  bool check2gpr(int* tokenCnt); // check instruction with 2 gprs
  bool check1csr1gpr(int* tokenCnt); // check instruction with 1 csr and 1 gpr
  bool check1gpr1csr(int* tokenCnt); // check instruction with 1 gpr and 1 csr
  int check1doperand1gpr(int* tokenCnt); // return -1 for bad syntax, 0 for memdir, 1 for regdir, 2 for regindir, 3 for regindir + addend, 4 for immed
  int check1gpr1doperand(int* tokenCnt); // return -1 for bad syntax, 0 for memdir, 1 for regdir, 2 for regindir, 3 for regindir + addend
  int isRegIndir(int* tokenCnt); // return -1 for bad syntax, 2 for regindir, 3 for regindir + addend,

  bool checkSymbolList(int tokenCnt); // check syntax for extern and global dirs
  int checkSymbolOrLiteralList(int* tokenCnt); // check syntax for word directive and return the number of args, -1 if bad syntax
  bool checkSymbol(int tokenCnt); // check syntax for a symbol
  int checkLiteral(int* tokenCnt); // check syntax for a hex or dec literal and return the value, -1 if bad syntax
  int checkString(int* tokenCnt); // check syntax for ascii and return the size of the string, -1 if bad syntax
  bool checkExpr(int* tokenCnt); // check syntax for equ
  
  //section-related functions
  void openSection(string name);
  void closeSection();

  //utility functions
  int hexStringToInt(string hex);
  string int12ToHexNoPrefix(int i);
  char gprIndex(string gpr);
  char csrIndex(string csr);

  //writing methods
  void write(char* chars, int size);
  void fill(char filler, int size);

  //directive and instruction processing PASS 2
  int addRelocationPCREL(Symbol* s, int offset);
  int addRelocationABS(Symbol* s, int offset);

  bool handleBranchOperand(int* tokenCnt, char* charr, int addrStart);
  void handleCondition(int* tokenCnt, char* charr);
  void handle1gpr(int* tokenCnt, char* charr);
  void handle2gpr(int* tokenCnt, char* charr);

  bool handleDataOperand(int* tokenCnt, char* charr);
  bool handle1doperand1gpr(int* tokenCnt, char* charr);
  void handle1indir1gpr(int* tokenCnt, char* charr);
  bool handle1indirAddend1gpr(int* tokenCnt, char* charr);
  bool handle1lit1gpr(int* tokenCnt, char* charr);
  bool handle12bitOperand(int* tokenCnt, char* charr);
  bool handle1gpr1doperand(int* tokenCnt, char* charr);
  void handle1gpr1indir(int* tokenCnt, char* charr);
  bool handle1gpr1indirAdded(int* tokenCnt, char* charr);
  void handle1gpr1csr(int* tokenCnt, char* charr);
  void handle1csr1gpr(int* tokenCnt, char* charr);

  bool handleGlobal(int* tokenCnt);
  bool handleWord(int* tokenCnt, char* charr);
  void handleSkip(int* tokenCnt);
  void handleAscii(int* tokenCnt);


};