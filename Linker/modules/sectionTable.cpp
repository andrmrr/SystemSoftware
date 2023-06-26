#include "sectionTable.h"

SectionTable* SectionTable::instance = nullptr;

SectionTable* SectionTable::getInstance(){
  if(instance == nullptr){
    initSectionTable();  
  } 
  return instance;
}

void SectionTable::initSectionTable(){
  if(instance != nullptr) delete instance;
  SectionTable* st = new SectionTable();
  instance = st;
}

void SectionTable::deleteInstance(){
  SectionTable* ins = getInstance();
  delete ins;
}

SectionTable::~SectionTable(){
  for(auto it = this->sections.begin(); it != this->sections.end(); it++){
    delete *it;
  }
}

int SectionTable::addSection(Symbol* ss, string file, vector<char> newData){
  Section* s = this->findSection(ss);
  if(s == nullptr) {
    s = new Section(ss);
    this->sections.push_back(s);
    s->addData(newData, file);
    return this->sections.size()-1;
  } else {
    s->addData(newData, file);
    for(int i = 0; i < this->sections.size(); i ++){
      if(this->sections[i] == s) return i;
    }
    return -1;
  }
}

int SectionTable::addEmptySection(Symbol* ss, string file){
    Section* s = new Section(ss);
    sections.push_back(s);
    return this->sections.size()-1;
}

int SectionTable::findSectionId(Symbol* ss){
  for(int i = 0; i < sections.size(); i ++){
    if(sections[i]->sectionSymbol->getName() == ss->getName()){
      return i;
    }
  }
  return -1;
}

Section* SectionTable::findSection(Symbol* ss){
  for(auto it = sections.begin(); it != sections.end(); it++){
    if((*it)->sectionSymbol == ss){
      return *it;
    }
  }
  return nullptr;
}

Section* SectionTable::getSection(int ndx){
  return sections[ndx];
}

// void SectionTable::writeToSection(Symbol* ss, char* chars, int size){
//   Section* section = findSection(ss);
//   if(section != nullptr){
//     section->write(chars, size);
//   }
// }

// void SectionTable::fillSection(Symbol* ss, char filler, int size){
//   Section* section = findSection(ss);
//   if(section != nullptr){
//     section->fill(filler, size);
//   }
// }

vector<Section*> SectionTable::getAllSections(){
  return sections;
}

int SectionTable::size() const {
  return sections.size() - 1; // ABS does not count
}

