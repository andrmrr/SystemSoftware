#include "symbol.h"
using namespace std;

ostream& operator<<(ostream& out, SymbolType st) {
  if(st == SymbolType::NOTYP) out << "NOTYP";
  else out << "SCTN";
  return out; 
}

int Symbol::globalId = 0;

Symbol::Symbol(string n, int val){
  this->id = globalId++;
  this->name = n;
  this->value = val;
}

Symbol::Symbol(string n, int sec, int val){
  this->id = globalId++;
  this->name = n;
  this->section = sec;
  this->value = val;
}

Symbol::Symbol(string n,  SymbolType t){
  this->id = globalId++;
  this->name = n;
  this->section = id;
  this->type = t;
}

void Symbol::setGlobal(){
  this->global = true;
}

bool Symbol::isGlobal(){
  return this->global;
}

void Symbol::setSize(int sz){
  this->size = sz;
}

int Symbol::getValue(){
  return this->value;
}

int Symbol::getId(){
  return this->id;
}

int Symbol::getSize(){
  return this->size;
}

SymbolType Symbol::getType(){
  return this->type;
}

int const Symbol::getSection(){
  return this->section;
}

string const Symbol::getName(){
  return this->name;
}

bool const Symbol::isSection(){
  return this->type == SymbolType::SECTION;
}