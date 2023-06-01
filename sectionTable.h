#pragma once

#include "section.h"
#include <vector>

class SectionTable{
  vector<Section*> sections;

  SectionTable(){}
  ~SectionTable();
  
public:
  static SectionTable* instance;
  
  //creation and destruction of the instance
  static SectionTable* getInstance();
  static void initSectionTable();
  static void deleteInstance();

  Section* findSection(Symbol* ss);
  Section* addSection(Symbol* ss);
  void writeToSection(Symbol* ss, char* chars, int size);
  void fillSection(Symbol* ss, char filler, int size);
  vector<Section*> getAllSections();
};