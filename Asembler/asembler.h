#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <exception>
#include "tokens.h"
#include "pool.h"
#include "symbolTable.h"
#include "relocationTable.h"
#include "sectionTable.h"

#define INSTRUCTION_SIZE 4

class Asembler {

  string outputFilePath;

  vector<Token> tokens;
  int lcounter;
  int line;

  Symbol* currSection;
  Pool* pool;
  SymbolTable* symbolTable;
  vector<RelocationTable*> relTables;
  SectionTable* secTable;
  bool error;

public:
  Asembler(vector<Token> tlist);
  Asembler(vector<Token> tlist, string outf);

  ~Asembler();

  void secondPassInit();
  int numRels();

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
  bool checkExpr(int tokenCnt); // check syntax for equ
  bool handleEqu(int* tokenCnt); //resolve equ directive
  
  //section-related functions
  void openSection(string name);
  void closeSection();

  //utility functions
  int hexStringToInt(string hex, bool prefix);
  string int12ToHexNoPrefix(int i);
  char gprIndex(string gpr);
  char csrIndex(string csr);

  //writing methods
  void write(char* chars, int size);
  void fill(char filler, int size);
  void writePool();

  //directive and instruction processing PASS 2
  int addRelocationPCREL(Symbol* s, int offset);
  int addRelocationABS(Symbol* s, int offset);

  bool handleOperand(int* tokenCnt, int offsFromLC);
  void handle1gpr(int* tokenCnt, char* gpr);
  void handle2gpr(int* tokenCnt, char* gpr1, char* gpr2);
  void handle1indir1gpr(int* tokenCnt, char* gpr1, char* gpr2);
  bool handle1indirAddend1gpr(int* tokenCnt, char* gpr1, char* gpr2, uint16_t* disp);
  bool handle12bitOperand(int* tokenCnt, uint16_t* disp);
  void handle1gpr1indir(int* tokenCnt, char* gpr1, char* gpr2);
  bool handle1gpr1indirAddend(int* tokenCnt, char* gpr1, char* gpr2, uint16_t* disp);

  void handle1gpr1csr(int* tokenCnt, char* gpr, char* csr);
  void handle1csr1gpr(int* tokenCnt, char* csr, char* gpr);

  bool handleGlobal(int* tokenCnt);
  bool handleWord(int* tokenCnt, char* charr);
  void handleSkip(int* tokenCnt);
  void handleAscii(int* tokenCnt);
  void skipToNewLine(int* tokenCnt);

  //handling instructions
  void writeInstruction(char* charr, char opcode, char a, char b, char c, uint16_t d);
  // void handleEmpty(char* charr); // HALT, INT, IRET
  void handlePush(int* tokenCnt, char* charr);
  void handlePop(int* tokenCnt, char* charr);
  void handleRet(int* tokenCnt, char* charr);
  bool handleCall(int* tokenCnt, char* charr);
  bool handleJmp(int* tokenCnt, char* charr);
  bool handleBeq(int* tokenCnt, char* charr);
  bool handleBne(int* tokenCnt, char* charr);
  bool handleBgt(int* tokenCnt, char* charr);
  void handleXchg(int* tokenCnt, char* charr);
  void handleAdd(int* tokenCnt, char* charr);
  void handleSub(int* tokenCnt, char* charr);
  void handleMul(int* tokenCnt, char* charr);
  void handleDiv(int* tokenCnt, char* charr);
  void handleNot(int* tokenCnt, char* charr);
  void handleAnd(int* tokenCnt, char* charr);
  void handleOr(int* tokenCnt, char* charr);
  void handleXor(int* tokenCnt, char* charr);
  void handleShl(int* tokenCnt, char* charr);
  void handleShr(int* tokenCnt, char* charr);
  bool handleLdmemdir(int* tokenCnt, char* charr);
  void handleLdregdir(int* tokenCnt, char* charr);
  void handleLdregind(int* tokenCnt, char* charr);
  bool handleLdregindadd(int* tokenCnt, char* charr);
  bool handleLdimmed(int* tokenCnt, char* charr);
  bool handleStmemdir(int* tokenCnt, char* charr);
  void handleStregdir(int* tokenCnt, char* charr);
  void handleStregind(int* tokenCnt, char* charr);
  bool handleStregindadd(int* tokenCnt, char* charr);
  void handleCsrrd(int* tokenCnt, char* charr);
  void handleCsrwr(int* tokenCnt, char* charr);

  //output methods
  void createTextFile();
  void createBinaryFile();
};