#pragma once
#include <string>
#include <ostream>

using namespace std;

enum class SymbolType{
  SECTION, NOTYP
};

ostream& operator<<(std::ostream& out, SymbolType tokenType);

class Symbol {
  static int globalId; // 0 za UND, 1 za ABS

  int id;
  uint32_t value = 0;
  SymbolType type = SymbolType::NOTYP;
  int size = 0;
  bool global = false;
  int section; 
  string name;
  string file;

public:
  Symbol(string n, int sec, int val, string f); // za regularne simbole
  Symbol(string n, SymbolType t, string f); // za sekcije

  // setters and getters
  void setGlobal();
  bool isGlobal();
  void setSize(int sz);
  int getValue();
  int getId();
  int getSize();
  SymbolType getType();
  int const getSection();
  string const getName();
  bool const isSection();
  bool isAbsolute();
  string getFile();
  void setFile(string file);
};