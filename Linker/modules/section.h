#pragma once

#include <vector>
#include "symbol.h"

class Section{
  Symbol* sectionSymbol;
  vector<vector<char>> data;

  Section() = delete;
  Section(Symbol* ss) : sectionSymbol(ss) {}

  // void write(char* chars, int size);
  // void fill(char filler, int size);
  void addData(vector<char> newData);

  friend class SectionTable;
public:
  vector<vector<char>> getData();
  Symbol* getSectionSymbol();
};