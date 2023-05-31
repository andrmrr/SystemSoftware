#pragma once

#include <vector>
#include "symbol.h"

class Section{
  Symbol* sectionSymbol;
  vector<char> data;

  Section() = delete;
  Section(Symbol* ss) : sectionSymbol(ss) {}

  void write(char* chars, int size);

  friend class SectionTable;
};