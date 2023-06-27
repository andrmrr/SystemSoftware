#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include "symbolTable.h"
#include "sectionTable.h"
#include "relocationTable.h"
#include "linkerExceptions.h"

using namespace std;

//makroi za pomeraja u zaglavlju sekcija
#define SH_STRNDX_OFFS 0
#define SH_TYPE_OFFS 2
#define SH_ADDR_OFFS 4
#define SH_OFFS_OFFS 8
#define SH_SIZE_OFFS 12
#define SH_LINK_OFFS 16
#define SH_INFO_OFFS 18
#define SH_ENTSIZE_OFFS 20

//makroi za pomeraje u zapisu simbola
#define ST_NAME_OFFS 0
#define ST_BIND_OFFS 4
#define ST_TYPE_OFFS 5
#define ST_SECNDX_OFFS 6
#define ST_VALUE_OFFS 8

class Linker {
  vector<string> input_file_names;
  string output_file_name;
  bool hex_output;
  vector<string> section_places;

  SymbolTable* symbolTable;
  SectionTable* secTable;
  vector<RelocationTable*> relTables;

public:
  Linker(vector<string> ifnames, bool is_hex, vector<string> places, string of);
  ~Linker();

  //focal functions
  void link();
  void linkHex();
  void linkRel();
  void load();
  //functions for hex output
  void setSectionAdresses();
  void solveRelocations();
  void hexOutput();
  void hexOutputTxt();
  //functions for relocatable output
  void fixRelocations();
  void relOutputTxt();
  void relOutputBin();

  //utility functions
  int getSectionOffset(Symbol* s, string file);
  vector<RelocationTable*> getSimilarRelTables(string secName);
  int numRels();
};