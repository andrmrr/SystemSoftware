#include "section.h"

// void Section::write(char* chars, int size){
//   for(int i = 0; i < size; i ++){
//     data.push_back(*(chars+i));
//   }
// }

// void Section::fill(char filler, int size){
//   for(int i = 0; i < size; i ++){
//     data.push_back(filler);
//   }
// }

vector<vector<char>> Section::getData(){
  return this->data;
}

Symbol* Section::getSectionSymbol(){
  return this->sectionSymbol;
}

void Section::addData(vector<char> newData){
  this->data.push_back(newData);
}