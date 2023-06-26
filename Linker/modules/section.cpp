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

void Section::setData(vector<char> newData, int ndx){
  this->data[ndx] = newData;
}

vector<string> Section::getFiles(){
  return this->files;
}

Symbol* Section::getSectionSymbol(){
  return this->sectionSymbol;
}

void Section::addData(vector<char> newData, string f){
  this->data.push_back(newData);
  this->files.push_back(f);
}

int Section::getAddress(){
  return this->address;
}

void Section::setAddress(int addr){
  this->address = addr;
}