#include "section.h"

void Section::write(char* chars, int size){
  for(int i = 0; i < size; i ++){
    data.push_back(*(chars+i));
  }
}

void Section::fill(char filler, int size){
  for(int i = 0; i < size; i ++){
    data.push_back(filler);
  }
}

vector<char> Section::getData(){
  return this->data;
}

void Section::setData(vector<char> newData){
  this->data = newData;
}

Symbol* Section::getSectionSymbol(){
  return this->sectionSymbol;
}
