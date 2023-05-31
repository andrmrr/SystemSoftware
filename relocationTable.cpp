#include "relocationTable.h"


RelocationTable::~RelocationTable(){
  for(auto it = this->relocations.begin(); it != this->relocations.end(); it++){
    delete *it;
  }
}

Relocation* RelocationTable::addAbsolute(int offs, string sym, int a){
  Relocation* r = new Relocation(offs, RelocationType::R_X86_64_32, sym, a);
  relocations.push_back(r);
  return r;
}

Relocation* RelocationTable::addRelative(int offs, string sym, int a){
  Relocation* r = new Relocation(offs, RelocationType::R_X86_64_PC32, sym, a);
  relocations.push_back(r);
  return r;
}

Symbol* RelocationTable::getSection(){
  return this->section;
}

