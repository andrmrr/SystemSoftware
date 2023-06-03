#include "asembler.h"
#include <iomanip>
using namespace std;

void Asembler::createTextFile(){
  string txtOutputPath = outputFilePath + ".txt";
  ofstream txtOutputFile(txtOutputPath);

  txtOutputFile << "Relocatable object file" << endl << endl;

  //Symbol Table
  txtOutputFile << "Symbol Table" << endl;
  txtOutputFile << "Num  Value    Size   Type  Bind  Ndx  Name" << endl;
  vector<Symbol*> symbols = symbolTable->getAllSymbols();
  for(auto it = symbols.begin(); it != symbols.end(); it++){
    Symbol* s = *it;
    string bind = (s->isGlobal()) ? "GLOB" : "LOC";
    string ndx = (s->getSection() == 0) ? "UND" : to_string(s->getSection());
    txtOutputFile << s->getId() << ":  " << " " << setfill('0') << setw(8)  << hex << s->getValue()
      << " " << setfill(' ') << setw(4) << s->getSize() << "  " << setw(5) << s->getType() << "  " 
      << setw(4) << bind << "  " << setw(3) << ndx << "  " << s->getName() << endl;
  }
  txtOutputFile << endl;

  //Sections
  vector<Section*> sections = secTable->getAllSections();
  for(auto it = sections.begin(); it != sections.end(); it++){
    Section* sec = *it;
    if(sec->getSectionSymbol()->getId() == 1) continue; // za ABS
    vector<char> data = sec->getData();

    txtOutputFile << "." << sec->getSectionSymbol()->getName() << endl;
    int cnt = 0;
    for(auto it = data.begin(); it != data.end(); it++){
      txtOutputFile << hex << setfill('0') << setw(2) << ((int)(*it) & 0x000000ff) << " ";
      cnt++;
      if(cnt == 8) {
        txtOutputFile << endl;
        cnt = 0;
      }
    }
    if(cnt != 0) txtOutputFile << endl;
  }
  txtOutputFile << endl;

  //Relocation Tables
  for(auto it = relTables.begin(); it != relTables.end(); it++) {
    RelocationTable* relTable = *it;
    if(relTable->getSection()->getId() == 1) continue; // za ABS
    if(relTable->getRelocations().empty()) continue;
    txtOutputFile << ".rela." << relTable->getSection()->getName() << endl;
    txtOutputFile << "Offset" << "    " << "Type" << "              " << "Symbol" << "   " << "Addend" << endl;
    vector<Relocation*> relocations = relTable->getRelocations();
    for(auto it = relocations.begin(); it != relocations.end(); it++){
      Relocation* r = *it;
      txtOutputFile << hex << setfill('0') << setw(8) << r->offset << "  " << setfill(' ') << setw(12)
        << r->type << "  " << setw(8) << r->symbol << "  " << setw(4) << r->addend << endl;
    }
  }

}


void createBinaryFile();