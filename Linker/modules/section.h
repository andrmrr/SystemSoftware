#pragma once

#include <vector>
#include "symbol.h"

class Section{
  Symbol* sectionSymbol;
  vector<vector<char>> data;
  vector<string> files;
  uint32_t address;

  Section() = delete;
  Section(Symbol* ss) : sectionSymbol(ss), address(0) {}

  // void write(char* chars, int size);
  // void fill(char filler, int size);
  void addData(vector<char> newData, string f);

  friend class SectionTable;
public:
  uint32_t getTotalSize();
  vector<vector<char>> getData();
  vector<char> getDataPart(int ndx);
  void setData(vector<char> newData, int ndx);
  vector<string> getFiles();
  Symbol* getSectionSymbol();
  uint32_t getAddress();
  void setAddress(uint32_t addr);
};