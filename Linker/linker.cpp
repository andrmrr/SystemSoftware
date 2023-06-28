#include "linker.h"


Linker::Linker(vector<string> ifnames, bool is_hex, vector<string> places, string of) {
  this->input_file_names = ifnames;
  this->hex_output = is_hex;
  this->section_places = places;
  if(of.empty()){
    if(is_hex){
      this->output_file_name = "output.hex";
    } else {
      this->output_file_name = "output.o";
    }
  } else {
    this->output_file_name = of;
  }
  SymbolTable::initSymbolTable();
  symbolTable = SymbolTable::getInstance();
  SectionTable::initSectionTable();
  secTable = SectionTable::getInstance();
  secTable->addEmptySection(new Symbol("ABS", SymbolType::SECTION, 0, ""), "");
}

Linker::~Linker() {
  for(auto it = relTables.begin(); it != relTables.end(); it ++){
    delete (*it);
  }
  this->symbolTable->deleteInstance();
  this->secTable->deleteInstance();
}

void Linker::link(){
  if(this->hex_output){
    linkHex();
  } else {
    linkRel();
  }
}

void Linker::linkHex(){
  load();
  setSectionAdresses();
  solveRelocations();
  hexOutput();
}

void Linker::linkRel(){
  load();
  fixRelocations();
  relOutputTxt();
  relOutputBin();
}

void Linker::load(){
  vector<string> defined;
  vector<string> unresolved;
  vector<Symbol*> curr_file_symbols;
  vector<Symbol*> curr_file_symbols2;
  char* nameTemp = (char*)malloc(100*sizeof(char));
  for(string ifname: input_file_names){
    curr_file_symbols.clear();
    curr_file_symbols2.clear();
    ifstream inputBinFile(ifname, ios_base::in | ios_base::binary);
    int curr_offset, curr_sh;

    uint32_t sec_header_offset;
    uint32_t flags;
    uint16_t sec_header_entry_size;
    uint16_t sec_header_num_entries;
    uint16_t string_section_index;
    uint32_t string_section_offset;

    inputBinFile.seekg(4);
    inputBinFile.read((char*)&sec_header_offset, sizeof(sec_header_offset));
    inputBinFile.read((char*)&flags, sizeof(flags));
    if(flags & 1 == 0) throw BadFileType();
    inputBinFile.seekg(18);
    inputBinFile.read((char*)&sec_header_entry_size, sizeof(sec_header_entry_size));
    inputBinFile.read((char*)&sec_header_num_entries, sizeof(sec_header_num_entries));
    inputBinFile.read((char*)&string_section_index, sizeof(string_section_index));
    inputBinFile.seekg(sec_header_offset + string_section_index * sec_header_entry_size + SH_OFFS_OFFS);
    inputBinFile.read((char*)&string_section_offset, sizeof(string_section_offset));

    curr_sh = 0;
    curr_offset = sec_header_offset + curr_sh * sec_header_entry_size;

    //Tabela simbola je prva sekcija
    inputBinFile.seekg(curr_offset);

    uint16_t sh_strndx;
    uint16_t sh_type;
    uint32_t sh_offs = 0x0;
    uint32_t sh_size = 0x0;
    uint16_t sh_link = 0x0; // indeks tabele stringova
    uint16_t sh_info = 0x0; // broj simbola u tabeli simbola
    uint32_t sh_entsize = 0x0;

    inputBinFile.seekg(curr_offset + SH_STRNDX_OFFS);
    inputBinFile.read((char*)(&sh_strndx), sizeof(sh_strndx));
    inputBinFile.seekg(curr_offset + SH_TYPE_OFFS);
    inputBinFile.read((char*)(&sh_type), sizeof(sh_type));
    if(sh_type != 2) throw InputFileError();
    inputBinFile.seekg(curr_offset + SH_OFFS_OFFS);
    inputBinFile.read((char*)(&sh_offs), sizeof(sh_offs));
    inputBinFile.seekg(curr_offset + SH_SIZE_OFFS);
    inputBinFile.read((char*)(&sh_size), sizeof(sh_size));
    inputBinFile.seekg(curr_offset + SH_LINK_OFFS);
    inputBinFile.read((char*)(&sh_link), sizeof(sh_link));
    inputBinFile.seekg(curr_offset + SH_INFO_OFFS);
    inputBinFile.read((char*)(&sh_info), sizeof(sh_info));
    inputBinFile.seekg(curr_offset + SH_ENTSIZE_OFFS);
    inputBinFile.read((char*)(&sh_entsize), sizeof(sh_entsize));

    curr_offset = sh_offs;
    uint32_t st_name;
    uint8_t st_bind;
    uint8_t st_type;
    uint16_t st_secndx; // r.b. u section_header, 0 za ABS, -1 za UND
    uint32_t st_value;
    Symbol* s;
    string name;
    for(int i = 0; i < sh_info; i ++){
      inputBinFile.seekg(curr_offset + i * sh_entsize);
      inputBinFile.read((char*)(&st_name), sizeof(st_name));
      inputBinFile.read((char*)(&st_bind), sizeof(st_bind));
      inputBinFile.read((char*)(&st_type), sizeof(st_type));
      inputBinFile.read((char*)(&st_secndx), sizeof(st_secndx));
      inputBinFile.read((char*)(&st_value), sizeof(st_value));
      inputBinFile.seekg(string_section_offset + st_name);
      inputBinFile.get(nameTemp, 100, '0');
      name = string(nameTemp);
      if(st_type == 3 || st_type == 1) { // ako je sekcija
        s = symbolTable->findSymbol(name);
        if(s != nullptr) { //vec je definisana ta sekcija
          curr_file_symbols2.push_back(s); //ubacuje se ovde da bismo brojali elemente tabele simbola zbog relokacionih zapisa
          continue;
        }
        s = symbolTable->addSection(name, st_secndx, ifname);
        curr_file_symbols.push_back(s);
        curr_file_symbols2.push_back(s);
      } else { // ako je globalni simbol koji nije sekcija
        if(st_secndx == -1 || st_secndx == 65535){ //ako je uvezeni simbol
          s = symbolTable->findSymbol(name);
          if(s == nullptr){ // nije vec definisan
            s = symbolTable->addSymbol(name, st_secndx, st_value, ifname);
            unresolved.push_back(name);
          }
          curr_file_symbols2.push_back(s);
        } else { // normalan simbol (neuvezen)
          for(auto it = defined.begin(); it != defined.end(); it++){
            if(it->compare(name) == 0) {
              throw MultipleDefinitionsError(name);
            }
          }
          s = symbolTable->findSymbol(name);
          if(s == nullptr){ // potpuno nov simbol
            s = symbolTable->addSymbol(name, st_secndx, st_value, ifname);
            if(st_bind == 1) s->setGlobal();
          } else { // ako vec postoji nedefinisani simbol u tabeli simbola
            s->setValue(st_value);
            s->setSection(st_secndx);
            s->setFile(ifname);
            if(st_bind == 1) s->setGlobal();
          }
          curr_file_symbols.push_back(s);
          curr_file_symbols2.push_back(s);
          defined.push_back(name);
          for(auto it = unresolved.begin(); it != unresolved.end();){
            if(it->compare(name) == 0) {
              it = unresolved.erase(it);
            } else {
              it++;
            }
          }
        }
      }
    }
    curr_sh++;
    curr_offset = sec_header_offset + curr_sh * sec_header_entry_size;

    //ucitavanje regularnih sekcija
    vector<char> secData;
    int currFileSectionId = 1;
    while(1){
      inputBinFile.seekg(curr_offset + SH_TYPE_OFFS);
      inputBinFile.read((char*)(&sh_type), sizeof(sh_type));
      if(sh_type != 1) { // prosli smo sve regularne sekcije
        inputBinFile.seekg(curr_offset);
        break;
      }
      inputBinFile.seekg(curr_offset + SH_OFFS_OFFS);
      inputBinFile.read((char*)(&sh_offs), sizeof(sh_offs));
      inputBinFile.seekg(curr_offset + SH_SIZE_OFFS);
      inputBinFile.read((char*)(&sh_size), sizeof(sh_size));
      inputBinFile.seekg(curr_offset + SH_STRNDX_OFFS);
      inputBinFile.read((char*)(&sh_strndx), sizeof(sh_strndx));
      inputBinFile.seekg(string_section_offset + sh_strndx);
      inputBinFile.get(nameTemp, 100, '0');
      name = string(nameTemp);

      inputBinFile.seekg(sh_offs);
      secData = vector<char>(sh_size, 0);
      inputBinFile.read((char*)(&secData[0]), sh_size);
      //Postavljamo polje section za sve simbole iz ove sekcije na indeks te sekcije u novoj tabeli simbola
      int sectionId = secTable->addSection(symbolTable->findSymbol(name), ifname, secData);
      for(auto it = curr_file_symbols.begin(); it != curr_file_symbols.end();){
        Symbol *ts = *it;
        if(ts->getSection() == currFileSectionId) {
          ts->setSection(sectionId);
          it = curr_file_symbols.erase(it);
        } else {
          it++;
        }
      }

      currFileSectionId++;
      secData.clear();
      curr_sh++;
      curr_offset = sec_header_offset + curr_sh * sec_header_entry_size;
    }

    //UCITAVANJE TABELA RELOKACIONIH ZAPISA
    uint32_t r_offset;
    uint32_t r_info_symbol;
    uint32_t r_addend;
    Section* refSec;
    RelocationTable* rt;
    string trueSym;
    int sym, info;
    while(1){
      inputBinFile.seekg(curr_offset + SH_TYPE_OFFS);
      inputBinFile.read((char*)(&sh_type), sizeof(sh_type));
      if(sh_type != 4) {
        break;
      }
      inputBinFile.seekg(curr_offset + SH_OFFS_OFFS);
      inputBinFile.read((char*)(&sh_offs), sizeof(sh_offs));
      inputBinFile.seekg(curr_offset + SH_SIZE_OFFS);
      inputBinFile.read((char*)(&sh_size), sizeof(sh_size));
      inputBinFile.seekg(curr_offset + SH_ENTSIZE_OFFS);
      inputBinFile.read((char*)(&sh_entsize), sizeof(sh_entsize));
      inputBinFile.seekg(curr_offset + SH_STRNDX_OFFS);
      inputBinFile.read((char*)(&sh_strndx), sizeof(sh_strndx));
      inputBinFile.seekg(string_section_offset + sh_strndx);
      inputBinFile.get(nameTemp, 100, '0');
      name = string(nameTemp);
      name = name.substr(6);
      s = symbolTable->findSymbol(name);
      refSec = secTable->findSection(s);
      rt = new RelocationTable(s, ifname);
      relTables.push_back(rt);

      curr_offset = sh_offs;
      for(int i = 0; i < sh_size/sh_entsize; i ++){
        inputBinFile.seekg(curr_offset + i * sh_entsize);
        inputBinFile.read((char*)(&r_offset), sizeof(r_offset));
        inputBinFile.read((char*)(&r_info_symbol), sizeof(r_info_symbol));
        inputBinFile.read((char*)(&r_addend), sizeof(r_addend));
        info = r_info_symbol >> 16;
        sym = r_info_symbol & 0xFFFF;
        trueSym = curr_file_symbols2[sym]->getName();
        //ABS->11, PC->2
        if(info == 11) {
          rt->addAbsolute(r_offset, trueSym, r_addend);
        } else if(info == 2){
          rt->addRelative(r_offset, trueSym, r_addend);
        } else throw InputFileError();
        
      }
      curr_sh++;
      curr_offset = sec_header_offset + curr_sh * sec_header_entry_size;
    }

    // cout << "Procitan fajl: " << ifname << endl;
  }
  free(nameTemp);
  if(this->hex_output){
    if(!unresolved.empty()){
      throw UnresolvedSymbolError(unresolved[0]);
    }
  }
}

void Linker::solveRelocations() {
  vector<Section*> sections = secTable->getAllSections();
  Section* currSection;
  vector<RelocationTable*> currRTables;
  for(auto it = sections.begin(); it != sections.end(); it++){
    currSection = *it;
    currRTables.clear();
    for(auto it2 = relTables.begin(); it2 != relTables.end(); it2++){
      if((*it2)->getSection()->getName() == currSection->getSectionSymbol()->getName()){
        currRTables.push_back(*it2);
      }
    }
    if(currRTables.empty()) continue;
    RelocationTable* rt;
    vector<char> sectionData;
    int localSecNdx;
    int sectionOffset; //ovo je offset u odnosu na sekciju simbola u rel. zapisu
    for(int i = 0; i < currRTables.size(); i++){
      rt = currRTables[i];
      for(int j = 0; j < currSection->getData().size(); j ++){
        if(currSection->getFiles()[j] == rt->getFile()){
          sectionData = currSection->getData()[j];
          localSecNdx = j;
          break;
        }
      }
      Relocation* r;
      Symbol* rs;
      uint32_t val;
      vector<Relocation*> rTemp = rt->getRelocations();
      for(auto it3 = rTemp.begin(); it3 != rTemp.end(); it3++){
        r = *it3;
        rs = symbolTable->findSymbol(r->symbol);
        sectionOffset = getSectionOffset(rs, rt->getFile());
        if(r->type == RelocationType::R_X86_64_32){
          if(rs->isGlobal()){
            val = sectionOffset + rs->getValue() + r->addend;
          } else {
            val = sectionOffset + r->addend;
          }
          sectionData[r->offset+0] = *((char*)&val+3);
          sectionData[r->offset+1] = *((char*)&val+2);
          sectionData[r->offset+2] = *((char*)&val+1);
          sectionData[r->offset+3] = *((char*)&val+0);
        } else { 
          // ne koristi se pc rel
        }
      }
      currSection->setData(sectionData, localSecNdx);
    }
  }
  // cout << "Relocations solved..." << endl;
}

int Linker::getSectionOffset(Symbol* s, string file){
  Section* sec = secTable->getSection(s->getSection());
  int offs = sec->getAddress();
  for(int i = 0; i < sec->getData().size(); i ++){
    if(s->isSection()){ // za sekcije kojih moze biti vise
      if(sec->getFiles()[i] == file){
        return offs;
      }
    } else { // za simbole koji su jedinstveni
      if(sec->getFiles()[i] == s->getFile()){
        return offs;
      }
    }
    offs += (sec->getData()[i]).size();
  }
  return offs;
}


void Linker::setSectionAdresses(){
  Section* sec, *startsAtZero = nullptr;
  vector<u_int32_t> segLow = {0};
  vector<u_int32_t> segHigh = {0xFFFFFFFF};
  string secName, saddr;
  u_int32_t addr, size;
  u_int32_t low, high;
  int delim;
  bool cantfit;
  //upisivanje zadatih adresa
  for(auto it = section_places.begin(); it != section_places.end(); it++){
    cantfit = true;
    delim = it->find('@');
    secName = it->substr(0, delim);
    addr = hexStringToUInt32(it->substr(delim+1), true);
    sec = secTable->findSection(symbolTable->findSymbol(secName));
    size = sec->getTotalSize();
    sec->setAddress(addr);
    if(addr == 0) startsAtZero = sec;
    for(auto itl = segLow.begin(), ith = segHigh.begin(); itl != segLow.end(), ith != segHigh.end();){
      low = *itl;
      high = *ith;
      if(addr >= low && addr+size-1 <= high){
        itl = segLow.erase(itl);
        ith = segHigh.erase(ith);
        if(addr > low){
          itl = segLow.insert(itl, low);
          itl++;
          segHigh.insert(ith, addr-1);
          ith++;
        }
        if(addr+size-1 < high){
          segLow.insert(itl, addr+size);
          itl++;
          segHigh.insert(ith, high);
          ith++;
        }
        cantfit = false;
        break;
      } else {
        itl++;
        ith++;
      }
    }
    if(cantfit) throw AddressOverlap(secName);
  }

  low = segLow.back();
  high = segHigh.back();

  //postavljanje adresa ostalih sekcija
  vector<Section*> sections = secTable->getAllSections();
  for(auto it = sections.begin(); it != sections.end(); it++){
    sec = *it;
    if(sec == startsAtZero || sec->getAddress() > 0 || sec->getTotalSize() == 0) continue;
    size = sec->getTotalSize();
    if(high-low+1 >= size){
      sec->setAddress(low);
      low += size;
    } else {
      throw AddressOverlap(sec->getSectionSymbol()->getName());
    }
  }
  // cout << "Zadali adrese" << endl;
}

void Linker::fixRelocations(){
  vector<Section*> sections = secTable->getAllSections();
  Section* currSection;
  vector<RelocationTable*> currRTables;
  for(auto it = sections.begin(); it != sections.end(); it++){
    currSection = *it;
    currRTables.clear();
    for(auto it2 = relTables.begin(); it2 != relTables.end(); it2++){
      if((*it2)->getSection()->getName() == currSection->getSectionSymbol()->getName()){
        currRTables.push_back(*it2);
      }
    }
    if(currRTables.empty()) continue;
    RelocationTable* rt;
    int oldSectionOffset; //ovo je offset stare sekcije u odnosu na pocetak nove
    for(int i = 0; i < currRTables.size(); i++){
      rt = currRTables[i];
      Relocation* r;
      Symbol* rs;
      uint32_t val;
      vector<Relocation*> rTemp = rt->getRelocations();
      for(auto it3 = rTemp.begin(); it3 != rTemp.end(); it3++){
        r = *it3;
        rs = symbolTable->findSymbol(r->symbol);
        oldSectionOffset = getSectionOffset(rt->getSection(), rt->getFile());
        if(r->type == RelocationType::R_X86_64_32){
          r->offset += oldSectionOffset;
        } else { 
          // ne koristi se pc rel
        }
      }
    }
  }
  // cout << "Relocations fixed..." << endl;
}

uint32_t Linker::hexStringToUInt32(string hex, bool prefix){
  uint32_t val = 0;
  char current;
  int i = (prefix) ? 2 : 0;
  for(i; i < hex.size(); i++){
    current = hex[i];
    val *= 16;
    switch(current){
      case '0':
        val += 0;
        break;
      case '1':
        val += 1;
        break;
      case '2':
        val += 2;
        break;
      case '3':
        val += 3;
        break;
      case '4':
        val += 4;
        break;
      case '5':
        val += 5;
        break;
      case '6':
        val += 6;
        break;
      case '7':
        val += 7;
        break;
      case '8':
        val += 8;
        break;
      case '9':
        val += 9;
        break;
      case 'a':
      case 'A':
        val += 10;
        break;
      case 'b':
      case 'B':
        val += 11;
        break;
      case 'c':
      case 'C':
        val += 12;
        break;
      case 'd':
      case 'D':
        val += 13;
        break;
      case 'e':
      case 'E':
        val += 14;
        break;
      case 'f':
      case 'F':
        val += 15;
        break;  
    }
  }
  return val;
}