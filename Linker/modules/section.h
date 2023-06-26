#pragma once

#include <vector>
#include "symbol.h"

class Section{
  Symbol* sectionSymbol;
  vector<vector<char>> data;
  vector<string> files;
  int address;

  Section() = delete;
  Section(Symbol* ss) : sectionSymbol(ss), address(0) {}

  // void write(char* chars, int size);
  // void fill(char filler, int size);
  void addData(vector<char> newData, string f);

  friend class SectionTable;
public:
  vector<vector<char>> getData();
  void setData(vector<char> newData, int ndx);
  vector<string> getFiles();
  Symbol* getSectionSymbol();
  int getAddress();
  void setAddress(int addr);
};