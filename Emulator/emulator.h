#pragma once

#include <string>
#include "cpu.h"

using namespace std;

class Emulator{
  string inputFileName;
  CPU* cpu;

public:
  Emulator(string ifName);
  ~Emulator();
  void  interpret();
  void emulate();

};