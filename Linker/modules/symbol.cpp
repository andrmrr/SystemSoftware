#include "symbol.h"
using namespace std;

ostream& operator<<(ostream& out, SymbolType st) {
  if(st == SymbolType::NOTYP) out << "NOTYP";
  else out << "SCTN";
  return out; 
}

int Symbol::globalId = 0;

Symbol::Symbol(string n, int sec, int val, string f){
  this->id = globalId++;
  this->name = n;
  this->section = sec;
  this->value = val;
  this->file = f;
}

Symbol::Symbol(string n, SymbolType t, int sec, string f){
  this->id = globalId++;
  this->name = n;
  this->section = sec;
  this->type = t;
  this->file = f;
}

Symbol::Symbol(string n, SymbolType t, string f){
  this->id = globalId++;
  this->name = n;
  this->section = id;
  this->type = t;
  this->file = f;
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

void Symbol::setValue(int val){
  this->value = val;
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

void Symbol::setSection(int secId){
  this->section = secId;
}

string const Symbol::getName(){
  return this->name;
}

bool const Symbol::isSection(){
  return this->type == SymbolType::SECTION;
}

bool Symbol::isAbsolute(){
  return this->section == 1;
}

string Symbol::getFile(){
  return this->file;
}
void Symbol::setFile(string file){
  this->file = file;
}