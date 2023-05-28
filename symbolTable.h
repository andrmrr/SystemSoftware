#pragma once

#include "symbol.h"
#include <vector>

class SymbolTable{
  static SymbolTable* instance;
  vector<Symbol*> symbols;

  SymbolTable(){}
  ~SymbolTable();
  
public:
  //creation and destruction of the instance
  static SymbolTable* getInstace();
  static void initSymbolTable();
  static void deleteInstance();

  Symbol* addSymbol(string n, int sec);
  Symbol* addAbsolute(string n, int sec, int val);
  Symbol* addSection(string n);

};