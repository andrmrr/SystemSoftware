#pragma once
#include <string>

using namespace std;

enum class SymbolType{
  SECTION, NOTYP
};

class Symbol {
  static int globalId; // 0 za UND, 1 za ABS

  int id;
  uint32_t value = 0;
  SymbolType type = SymbolType::NOTYP;
  int size = 0;
  bool global = false;
  int section; 
  string name;

public:
  Symbol(string n, int val); // za equ - idu u abs sekciju
  Symbol(string n, int sec, int val); // za regularne simbole
  Symbol(string n, SymbolType t); // za sekcije

  // setters and getters
  void setGlobal();
  void setSize(int sz);
  int getSection();
  string getName();
};