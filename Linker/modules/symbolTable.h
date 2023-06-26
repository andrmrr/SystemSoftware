#pragma once

#include "symbol.h"
#include <vector>

class SymbolTable{
  static SymbolTable* instance;
  vector<Symbol*> symbols;

  SymbolTable(){}
  ~SymbolTable();
  
public:
  int size() const;

  //creation and destruction of the instance
  static SymbolTable* getInstance();
  static void initSymbolTable();
  static void deleteInstance();

  // Symbol* addAbsolute(string n, int val);
  Symbol* addSymbol(string n, int sec, int val, string file);
  Symbol* addSection(string n, int sec, string file);

  //record methods
  Symbol* findSymbol(string name);
  Symbol* findSymbol(int id);
  vector<Symbol*> getSections();
  vector<Symbol*> getAllSymbols();

};

class AlreadyDefinedException : public exception {
  string symbol;
  public:
    AlreadyDefinedException(string sym): symbol(sym) {}
    const char* what() const noexcept override {
      return "The symbol is already defined: ";
    }
    string whatSymbol() const noexcept {
      return this->what() + symbol;
    }
};