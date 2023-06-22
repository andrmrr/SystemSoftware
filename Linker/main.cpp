#include <iostream>
#include "linker.h"

using namespace std;

int main(int argc, char** argv){
  cout << "Linker" << endl;

  vector<string> names;
  names.push_back("test_files/exampleCeo.o");
  names.push_back("test_files/primer1.o");

  Linker ld(names);
  ld.solveReferences();

  return 0;
}