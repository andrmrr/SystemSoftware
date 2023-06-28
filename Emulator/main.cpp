#include <iostream>
#include "emulator.h"

int main(int argc, char** argv){

  if(argc != 2){
    cout << "Za emulator se mora navesti ime ulaznog fajla." << endl;
    return 1;
  }

  string ifName(argv[1]);


  // Emulator em("test_files/output.hex");
  Emulator em(ifName);
  em.emulate();

  return 0;
}