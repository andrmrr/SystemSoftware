#include "symbolTable.h"

SymbolTable* SymbolTable::instance = nullptr;

SymbolTable* SymbolTable::getInstance(){
  if(instance == nullptr){
    initSymbolTable();  
  } 
  return instance;
}

void SymbolTable::initSymbolTable(){
  if(instance != nullptr) delete instance;
  SymbolTable* st = new SymbolTable();
  st->addSymbol("UND", 0, 0);
  Symbol* s = st->addSection("ABS");
  s->setGlobal();
  instance = st;
}

void SymbolTable::deleteInstance(){
  SymbolTable* ins = getInstance();
  delete ins;
}

SymbolTable::~SymbolTable(){
  for(auto it = this->symbols.begin(); it != this->symbols.end(); it++){
    delete *it;
  }
}

Symbol* SymbolTable::addAbsolute(string n, int val){
  Symbol* s = new Symbol(n, val); 
  symbols.push_back(s);
  return s;
}
Symbol* SymbolTable::addSymbol(string n, int sec, int val){
  Symbol* s = new Symbol(n, sec, val); 
  symbols.push_back(s);
  return s;
}
Symbol* SymbolTable::addSection(string n){
  Symbol* s = new Symbol(n, SymbolType::SECTION); 
  symbols.push_back(s);
  return s;
}

Symbol* SymbolTable::findSymbol(string name){
  for(auto it = symbols.begin(); it != symbols.end(); it++){
    if(((*it)->getName()).compare(name) == 0){
      return *it;
    }
  }
  return nullptr;
}

Symbol* SymbolTable::findSymbol(int id){
  for(auto it = symbols.begin(); it != symbols.end(); it++){
    if((*it)->getId() == id){
      return *it;
    }
  }
  return nullptr;
}

vector<Symbol*> SymbolTable::getSections(){
  vector<Symbol*> sections;
  for(auto it = symbols.begin(); it != symbols.end(); it++){
    if((*it)->isSection()){
      sections.push_back(*it);
    }
  }
  return sections;
}