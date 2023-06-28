#pragma once

#include <vector>
#include <string>

using namespace std;

class Pool{
  static Pool* instance;
  vector<uint32_t> literals;
  vector<vector<uint32_t>> literalsOffsets;
  vector<string> symbols;
  vector<vector<uint32_t>> symbolsOffsets;
  

  Pool(){}
  ~Pool();
  
public:
  int totalSize() const;

  //creation and destruction of the instance
  static Pool* getInstance();
  static void initPool();
  static void deleteInstance();

  //literals and symbols
  void addLiteral(uint32_t literal, uint32_t offset);
  void addSymbol(string symbol, uint32_t offset);
  uint32_t getLiteral();
  vector<uint32_t> getLiteralOffsets();
  string getSymbol();
  vector<uint32_t> getSymbolOffsets();
  
};