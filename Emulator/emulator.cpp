#include "emulator.h"

Emulator::Emulator(string ifName){
  this->inputFileName = ifName;
  this->cpu = new CPU();
}

Emulator::~Emulator(){
  delete cpu;
}

void Emulator::emulate(){
  cpu->loadMemory(inputFileName);
  cpu->run();
  cpu->print();
}