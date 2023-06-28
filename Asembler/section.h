#pragma once

#include <vector>
#include "symbol.h"

class Section{
  Symbol* sectionSymbol;
  vector<char> data;

  Section() = delete;
  Section(Symbol* ss) : sectionSymbol(ss) {}

  void write(char* chars, int size);
  void fill(char filler, int size);

  friend class SectionTable;
public:
  vector<char> getData();
  void setData(vector<char> newData);
  Symbol* getSectionSymbol();
};