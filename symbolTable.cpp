#include "symbolTable.h"

SymbolTable* SymbolTable::instance = nullptr;

SymbolTable* SymbolTable::getInstace(){
  if(instance == nullptr){
    initSymbolTable();  
  } 
  return instance;
}

void SymbolTable::initSymbolTable(){
  if(instance != nullptr) delete instance;
  SymbolTable* st = new SymbolTable();
  st->addSymbol("UND", 0);
  st->addSection("ABS");
  instance = st;
}

void SymbolTable::deleteInstance(){
  SymbolTable* ins = getInstace();
  delete ins;
}

SymbolTable::~SymbolTable(){
  for(auto it = this->symbols.begin(); it != this->symbols.end(); it++){
    delete *it;
  }
}

  Symbol* SymbolTable::addSymbol(string n, int sec){
    Symbol* s = new Symbol(n, sec); 
    symbols.push_back(s);
    return s;
  }
  Symbol* SymbolTable::addAbsolute(string n, int sec, int val){
    Symbol* s = new Symbol(n, sec, val); 
    symbols.push_back(s);
    return s;
  }
  Symbol* SymbolTable::addSection(string n){
    Symbol* s = new Symbol(n, SymbolType::SECTION); 
    symbols.push_back(s);
    return s;
  }
