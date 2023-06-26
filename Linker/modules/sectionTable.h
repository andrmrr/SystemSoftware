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

  int size() const;
  Section* findSection(Symbol* ss);
  Section* getSection(int id);
  int findSectionId(Symbol* ss);
  int addSection(Symbol* ss, string file, vector<char> newData);
  int addEmptySection(Symbol* ss, string file);
  // void writeToSection(Symbol* ss, char* chars, int size);
  // void fillSection(Symbol* ss, char filler, int size);
  vector<Section*> getAllSections();
};