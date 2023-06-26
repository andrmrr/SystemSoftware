#include <iostream>
#include "linker.h"

using namespace std;

int main(int argc, char** argv){
  cout << "Linker" << endl;

  vector<string> names;
  names.push_back("test_files/example6.o");
  names.push_back("test_files/primer1.o");
  names.push_back("test_files/exampleCeo.o");

  vector<string> places;
  places.push_back("prva@3840"); //0x0F00
  places.push_back("skokovi@100");
  places.push_back("trecaa@256"); //0x0100
  places.push_back("treca@0");
  // places.push_back("cetvrtaa@110");

  try{
    //izvrsavanje linkera
    Linker ld(names, true, places);
    ld.linkHex();

  } catch(const SymbolError& se){
    cout << se.whatSymbol();
  } catch(exception e){
    cout << e.what();
  }

  return 0;
}