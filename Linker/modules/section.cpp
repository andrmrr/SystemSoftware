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

uint32_t Section::getTotalSize(){
  int sz = 0;
  for(auto it = data.begin(); it != data.end(); it++){
    sz += it->size();
  }
  return sz;
}
vector<char> Section::getDataPart(int ndx){
  return data[ndx];
}

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

uint32_t Section::getAddress(){
  return this->address;
}

void Section::setAddress(uint32_t addr){
  this->address = addr;
}