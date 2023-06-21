#pragma once
#include <vector>
#include <string>
#include "symbol.h"

using namespace std;

enum RelocationType {R_X86_64_32 , R_X86_64_PC32};

ostream& operator<<(ostream& out, const RelocationType rt);

struct Relocation {
  Relocation() = delete;
  Relocation(int o, RelocationType t, string s, int a) : offset(o), type(t), symbol(s), addend(a) {}
  int offset;
  RelocationType type;
  string symbol;
  int addend;
};

class RelocationTable {

  Symbol* section;
  vector<Relocation*> relocations;

  
public:
  RelocationTable(Symbol* s) : section(s) {}
  ~RelocationTable();

  int size();

  Relocation* addAbsolute(int offs, string sym, int a);
  Relocation* addRelative(int offs, string sym, int a);

  Symbol* getSection();
  vector<Relocation*> getRelocations();

};