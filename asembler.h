#pragma once

#include <list>
#include "tokens.h"

class Asembler {

  list<Token> tokens;
  int lcounter;

public:
  Asembler(list<Token> tlist);

  void firstPass();
  void secondPass();

  void incCounter(int inc);
};