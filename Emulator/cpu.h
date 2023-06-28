#pragma once

#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

typedef unsigned char Byte;
typedef Byte Instruction[4];
// typedef struct {
//   Byte a, sp, x, pc, ir;
//   bool z, p, c;
// } Processor;

// typedef enum { running, finished, no_data, bad_data, bad_op } Status;

class CPU {
  map<uint32_t, Byte> memory;
  //private methods for memory manipulation

  void writeByte(uint32_t addr, Byte b);
  Byte readByte(uint32_t addr);

public:
  CPU();
  ~CPU();
  void loadMemory(string ifName);

  //utility functions
  vector<string> split(const string line, char delim);
  uint32_t hexStringToUInt32(string hex, bool prefix);
};