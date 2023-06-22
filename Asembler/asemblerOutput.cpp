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
    if(relTable->size() == 0) continue;
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


void Asembler::createBinaryFile(){
  string binOutputPath = outputFilePath + ".o";
  ofstream binOutputFile(binOutputPath, ios::out | ios::binary);
  int curr_offset, prev_offset;

  //ELF header
  uint32_t prog_header_offset = 0;                                                // 4B
  uint32_t sec_header_offset = 24;                                                // 4B
  uint32_t flags = 1; // 1->REL, 0->EXE                                           // 4B
  uint16_t elf_header_size = 24;                                                  // 2B
  uint16_t prog_header_entry_size = 0;                                            // 2B
  uint16_t prog_header_num_entries = 0;                                           // 2B
  uint16_t sec_header_entry_size = 24;                                            // 2B
  uint16_t sec_header_num_entries = secTable->size() + numRels() + 1 + 1;         // 2B   // 1 zbog Tabele simbola +1 zbog Tabele Stringovas
  uint16_t string_section_index = sec_header_num_entries - 1;                     // 2B

  
  
  binOutputFile.write((char*)(&prog_header_offset), sizeof(prog_header_offset));
  binOutputFile.write((char*)(&sec_header_offset), sizeof(sec_header_offset));
  binOutputFile.write((char*)(&flags), sizeof(flags));
  binOutputFile.write((char*)(&elf_header_size), sizeof(elf_header_size));
  binOutputFile.write((char*)(&prog_header_entry_size), sizeof(prog_header_entry_size));
  binOutputFile.write((char*)(&prog_header_num_entries), sizeof(prog_header_num_entries));
  binOutputFile.write((char*)(&sec_header_entry_size), sizeof(sec_header_entry_size));
  binOutputFile.write((char*)(&sec_header_num_entries), sizeof(sec_header_num_entries));
  binOutputFile.write((char*)(&string_section_index), sizeof(string_section_index));

  
  //Section header table
  string names = "0";
  int curr_sh = 0;
  int sh_strndx_offs = 0, sh_type_off = 2, sh_addr_offs = 4, sh_offs_offs = 8;
  int sh_size_offs = 12, sh_link_offs = 16, sh_info_off = 18, sh_entsize_offs = 20;
  uint16_t sh_strndx = 0x0;   //2B
  uint16_t sh_type = 0x0;     //2B
  uint32_t sh_addr = 0x0;     //4B
  uint32_t sh_offs = 0x0;     //4B
  uint32_t sh_size = 0x0;     //4B
  uint16_t sh_link = 0x0;     //2B
  uint16_t sh_info = 0x0;     //2B
  uint32_t sh_entsize = 0x0;  //4B

  for(int i = 0; i < sec_header_num_entries; i ++){
    binOutputFile.write((char*)(&sh_strndx), sizeof(sh_strndx));
    binOutputFile.write((char*)(&sh_type), sizeof(sh_type));
    binOutputFile.write((char*)(&sh_addr), sizeof(sh_addr));
    binOutputFile.write((char*)(&sh_offs), sizeof(sh_offs));
    binOutputFile.write((char*)(&sh_size), sizeof(sh_size));
    binOutputFile.write((char*)(&sh_link), sizeof(sh_link));
    binOutputFile.write((char*)(&sh_info), sizeof(sh_info));
    binOutputFile.write((char*)(&sh_entsize), sizeof(sh_entsize));
  }
  
  //SECTIONS
  curr_sh = 0;
  curr_offset = sec_header_offset + sec_header_entry_size * sec_header_num_entries;
  prev_offset = 0;
  uint32_t st_name;
  uint8_t st_bind;
  uint8_t st_type;
  uint16_t st_secndx; // r.b. u section_header = r.b. tabeli sekcija (-1 zbog abs, +1 zbog tabele simbola)
  uint32_t st_value;
  //Symbol Table
  int sym_tab_sh_entry = curr_sh;
  for(Symbol* s : symbolTable->getAllSymbols()){
    if(s->getId() == 0 || s->getId() == 1) continue; //und i abs se ne izvoze
    st_name = names.size();
    names += s->getName() + "0";
    st_bind = s->isGlobal() ? 1 : 0;
    st_type = s->isSection() ? 3 : 0;
    st_secndx = secTable->findSectionId(symbolTable->findSymbol(s->getSection()));

    st_value = s->getValue();

    binOutputFile.write((char*)(&st_name), sizeof(st_name));
    binOutputFile.write((char*)(&st_bind), sizeof(st_bind));
    binOutputFile.write((char*)(&st_type), sizeof(st_type));
    binOutputFile.write((char*)(&st_secndx), sizeof(st_secndx));
    binOutputFile.write((char*)(&st_value), sizeof(st_value));
  }
  prev_offset = curr_offset;
  curr_offset = binOutputFile.tellp();

  //upisi sve kako treba u section header
  sh_strndx = names.size();
  names += "symtab0";
  sh_type = 0x2;
  sh_addr = 0x0;
  sh_offs = prev_offset;
  sh_size = curr_offset - prev_offset;
  sh_link = string_section_index;
  sh_info = symbolTable->size()-2; //ne racunamo UND i ABS
  sh_entsize = 0xC;
  binOutputFile.seekp(sec_header_offset + curr_sh * sec_header_entry_size);
  binOutputFile.write((char*)(&sh_strndx), sizeof(sh_strndx));
  binOutputFile.write((char*)(&sh_type), sizeof(sh_type));
  binOutputFile.write((char*)(&sh_addr), sizeof(sh_addr));
  binOutputFile.write((char*)(&sh_offs), sizeof(sh_offs));
  binOutputFile.write((char*)(&sh_size), sizeof(sh_size));
  binOutputFile.write((char*)(&sh_link), sizeof(sh_link));
  binOutputFile.write((char*)(&sh_info), sizeof(sh_info));
  binOutputFile.write((char*)(&sh_entsize), sizeof(sh_entsize));
  binOutputFile.seekp(curr_offset);

  //Regular Sections
  for(Section* sec : secTable->getAllSections()){
    if(sec->getSectionSymbol()->getId() == 1) continue; // za ABS
    curr_sh++;

    vector<char> data = sec->getData();
    binOutputFile.write((char*)(&data[0]), data.size());
    
    //allignment to 4
    int rem, tmp = 0;
    rem = binOutputFile.tellp() % 4;
    if(rem > 0) binOutputFile.write((char*)(&tmp), 4-rem);

    prev_offset = curr_offset;
    curr_offset = binOutputFile.tellp();

    //upisi sve kako treba u section header
    sh_strndx = names.find(sec->getSectionSymbol()->getName());
    sh_type = 0x1;
    sh_addr = 0x0;
    sh_offs = prev_offset;
    sh_size = curr_offset - prev_offset;
    sh_link = 0x0;
    sh_info = 0x0;
    sh_entsize = 0x0;
    binOutputFile.seekp(sec_header_offset + curr_sh * sec_header_entry_size);
    binOutputFile.write((char*)(&sh_strndx), sizeof(sh_strndx));
    binOutputFile.write((char*)(&sh_type), sizeof(sh_type));
    binOutputFile.write((char*)(&sh_addr), sizeof(sh_addr));
    binOutputFile.write((char*)(&sh_offs), sizeof(sh_offs));
    binOutputFile.write((char*)(&sh_size), sizeof(sh_size));
    binOutputFile.write((char*)(&sh_link), sizeof(sh_link));
    binOutputFile.write((char*)(&sh_info), sizeof(sh_info));
    binOutputFile.write((char*)(&sh_entsize), sizeof(sh_entsize));
    binOutputFile.seekp(curr_offset);
  }

  //Relocation Tables
  uint32_t r_offset;
  uint32_t r_info_symbol;
  uint32_t r_addend;
  for(RelocationTable* rt : relTables){
    if(rt->size() == 0) continue;
    curr_sh++;

    for(Relocation* r : rt->getRelocations()){
      r_offset = r->offset;
      r_info_symbol = (r->type == RelocationType::R_X86_64_32) ? 11 : 2; //ABS->11, PC->2
      r_info_symbol <<= 16;
      r_info_symbol |= (uint32_t)symbolTable->findSymbol(r->symbol)->getId() - 2;
      r_addend = r->addend;

      binOutputFile.write((char*)(&r_offset), sizeof(r_offset));
      binOutputFile.write((char*)(&r_info_symbol), sizeof(r_info_symbol));
      binOutputFile.write((char*)(&r_addend), sizeof(r_addend));
    }
    prev_offset = curr_offset;
    curr_offset = binOutputFile.tellp();

    //upisi sve kako treba u section header
    sh_strndx = names.size();
    names += ".rela." + rt->getSection()->getName() + "0";
    sh_type = 0x4;
    sh_addr = 0x0;
    sh_offs = prev_offset;
    sh_size = curr_offset - prev_offset;
    sh_link = sym_tab_sh_entry;
    sh_info = secTable->findSectionId(rt->getSection());
    sh_entsize = sizeof(r_offset) + sizeof(r_info_symbol) + sizeof(r_addend);
    binOutputFile.seekp(sec_header_offset + curr_sh * sec_header_entry_size);
    binOutputFile.write((char*)(&sh_strndx), sizeof(sh_strndx));
    binOutputFile.write((char*)(&sh_type), sizeof(sh_type));
    binOutputFile.write((char*)(&sh_addr), sizeof(sh_addr));
    binOutputFile.write((char*)(&sh_offs), sizeof(sh_offs));
    binOutputFile.write((char*)(&sh_size), sizeof(sh_size));
    binOutputFile.write((char*)(&sh_link), sizeof(sh_link));
    binOutputFile.write((char*)(&sh_info), sizeof(sh_info));
    binOutputFile.write((char*)(&sh_entsize), sizeof(sh_entsize));
    binOutputFile.seekp(curr_offset);
  }

  //Tabela Stringova
  curr_sh++;

  int str_tab_ndx = names.size();
  names += ".strtab0";
  binOutputFile.write(names.c_str(), names.size());
  //allignment to 4
  int rem, tmp = 0;
  rem = binOutputFile.tellp() % 4;
  if(rem > 0) binOutputFile.write((char*)(&tmp), 4-rem);

  prev_offset = curr_offset;
  curr_offset = binOutputFile.tellp();

  //upisi sve kako treba u section header
  sh_strndx = str_tab_ndx;
  sh_type = 0x3;
  sh_addr = 0x0;
  sh_offs = prev_offset;
  sh_size = curr_offset - prev_offset;
  sh_link = 0x0;
  sh_info = 0x0;
  sh_entsize = 0x0;
  binOutputFile.seekp(sec_header_offset + curr_sh * sec_header_entry_size);
  binOutputFile.write((char*)(&sh_strndx), sizeof(sh_strndx));
  binOutputFile.write((char*)(&sh_type), sizeof(sh_type));
  binOutputFile.write((char*)(&sh_addr), sizeof(sh_addr));
  binOutputFile.write((char*)(&sh_offs), sizeof(sh_offs));
  binOutputFile.write((char*)(&sh_size), sizeof(sh_size));
  binOutputFile.write((char*)(&sh_link), sizeof(sh_link));
  binOutputFile.write((char*)(&sh_info), sizeof(sh_info));
  binOutputFile.write((char*)(&sh_entsize), sizeof(sh_entsize));
  binOutputFile.seekp(curr_offset);
}

int Asembler::numRels(){
  int cnt = 0;
  for(RelocationTable* r : relTables){
    if(r->size() > 0) cnt++;
  }
  return cnt;
}
