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
  static SymbolTable* getInstance();
  static void initSymbolTable();
  static void deleteInstance();

  Symbol* addAbsolute(string n, int sec);
  Symbol* addSymbol(string n, int sec, int val);
  Symbol* addSection(string n);

  //record methods
  Symbol* findSymbol(string name);
  Symbol* findSymbol(int id);
  vector<Symbol*> getSections();
  vector<Symbol*> getAllSymbols();

};