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

Section* SectionTable::addSection(Symbol* ss){
  Section* s = new Section(ss);
  sections.push_back(s);
  return s;
}

Section* SectionTable::findSection(Symbol* ss){
  for(auto it = sections.begin(); it != sections.end(); it++){
    if((*it)->sectionSymbol == ss){
      return *it;
    }
  }
  return nullptr;
}

void SectionTable::writeToSection(Symbol* ss, char* chars, int size){
  Section* section = findSection(ss);
  if(section != nullptr){
    section->write(chars, size);
  }
}

void SectionTable::fillSection(Symbol* ss, char filler, int size){
  Section* section = findSection(ss);
  if(section != nullptr){
    section->fill(filler, size);
  }
}

vector<Section*> SectionTable::getAllSections(){
  return sections;
}



