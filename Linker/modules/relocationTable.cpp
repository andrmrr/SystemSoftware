#include "relocationTable.h"

ostream& operator<<(ostream& out, const RelocationType rt){
  if(rt == RelocationType::R_X86_64_32) out << "R_X86_64_32";
  else out << "R_X86_64_PC32";
  return out;
}

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

string RelocationTable::getFile(){
  return this->file;
}

Symbol* RelocationTable::getSection(){
  return this->section;
}

vector<Relocation*> RelocationTable::getRelocations(){
  return this->relocations;
}

int RelocationTable::size(){
  return this->relocations.size();
}