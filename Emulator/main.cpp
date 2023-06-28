#include <iostream>
#include "emulator.h"

int main(int argc, char** argv){

  Emulator em("test_files/output.hex");
  em.emulate();

  return 0;
}