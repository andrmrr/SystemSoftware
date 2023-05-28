#include "symbol.h"

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

void Symbol::setSize(int sz){
  this->size = sz;
}

int Symbol::getSection(){
  return this->section;
}

string Symbol::getName(){
  return this->name;
}