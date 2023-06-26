#include <iostream>
#include "linker.h"

using namespace std;

int main(int argc, char** argv){
  cout << "Linker" << endl;

  vector<string> names;
  names.push_back("test_files/example6.o");
  names.push_back("test_files/primer1.o");
  names.push_back("test_files/exampleCeo.o");

  try{
    //izvrsavanje linkera
    Linker ld(names, true);
    ld.solveReferences();

  } catch(const SymbolError& se){
    cout << se.whatSymbol();
  } catch(exception e){
    cout << e.what();
  }

  return 0;
}