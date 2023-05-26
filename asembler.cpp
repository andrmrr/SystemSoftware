#include "asembler.h"

Asembler::Asembler(list<Token> tl){
  lcounter = 0;
  tokens = tl;
}

void Asembler::incCounter(int inc){
  lcounter += inc;
}

void Asembler::firstPass(){
  
}