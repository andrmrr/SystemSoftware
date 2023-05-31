#include "section.h"

void Section::write(char* chars, int size){
  for(int i = 0; i < size; i ++){
    data.push_back(*(chars+i));
  }
}