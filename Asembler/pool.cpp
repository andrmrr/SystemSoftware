#include "pool.h"

Pool* Pool::instance = nullptr;

Pool* Pool::getInstance(){
  if(instance == nullptr){
    initPool();  
  } 
  return instance;
}

void Pool::initPool(){
  if(instance != nullptr) delete instance;
  Pool* st = new Pool();
  // st->addSymbol("UND", 0, 0);
  // Symbol* s = st->addSection("ABS");
  // s->setGlobal();
  instance = st;
}

void Pool::deleteInstance(){
  Pool* ins = getInstance();
  delete ins;
}

Pool::~Pool(){

}

int Pool::totalSize() const {
  return this->symbols.size() + this->literals.size();
}

void Pool::addLiteral(uint32_t literal, uint32_t offset){
  auto itl = literals.begin();
  auto ito = literalsOffsets.begin();
  for(; itl != literals.end() && ito != literalsOffsets.end(); itl++, ito++){
    if(*itl == literal) {
      ito->push_back(offset);
      return;
    }
  }
  literals.push_back(literal);
  vector<uint32_t> newvec = {offset};
  literalsOffsets.push_back(newvec);
}

void Pool::addSymbol(string symbol, uint32_t offset){
  auto its = symbols.begin();
  auto ito = symbolsOffsets.begin();
  for(; its != symbols.end() && ito != symbolsOffsets.end(); its++, ito++){
    if(*its == symbol) {
      ito->push_back(offset);
      return;
    }
  }
  symbols.push_back(symbol);
  vector<uint32_t> newvec = {offset};
  symbolsOffsets.push_back(newvec);
}

uint32_t Pool::getLiteral(){
  if(literals.empty()) return 0;
  uint32_t lit = literals.back();
  literals.pop_back();
  return lit;
}
vector<uint32_t> Pool::getLiteralOffsets(){
  if(literalsOffsets.empty()) return vector<uint32_t>();
  vector<uint32_t> offsets = literalsOffsets.back();
  literalsOffsets.pop_back();
  return offsets;
}
string Pool::getSymbol(){
  if(symbols.empty()) return "";
  string sym = symbols.back();
  symbols.pop_back();
  return sym;
}
vector<uint32_t> Pool::getSymbolOffsets(){
  if(symbolsOffsets.empty()) return vector<uint32_t>();
  vector<uint32_t> offsets = symbolsOffsets.back();
  symbolsOffsets.pop_back();
  return offsets;
}