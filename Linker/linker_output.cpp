#include "linker.h"
#include <iomanip>
#include <set>

using namespace std;

void Linker::hexOutput(){
  string binOutputPath = output_file_name;
  ofstream binOutputFile(binOutputPath, ios::out | ios::binary);
  vector<Section *> sections = secTable->getAllSections();
  int cnt, addr;
  uint32_t temp;
  uint8_t charr;
  Section *sec;
  vector<char> data;
  vector<char> tmpData;
  for (auto it = sections.begin(); it != sections.end(); it++){
    sec = *it;
    for (int i = 0; i < sec->getData().size(); i++)
    {
      tmpData = sec->getDataPart(i);
      data.insert(data.end(), tmpData.begin(), tmpData.end());
    }
    cnt = 0;
    addr = sec->getAddress();   
    for (auto it = data.begin(); it != data.end(); it++){
      if (cnt % 8 == 0){
        temp = addr + cnt;
        binOutputFile.write((char*)(&temp), sizeof(temp));
      }

      charr = *it;
      binOutputFile.write((char*)(&charr), sizeof(charr));
      cnt++;

    }
    // if (cnt % 8 != 0) txtOutputFile << endl;
    
  }
  // txtOutputFile << endl;
}

void Linker::hexOutputTxt(){
  // string binOutputPath = output_file_name;
  // ofstream binOutputFile(binOutputPath, ios::out | ios::binary);
  string txtOutputPath = output_file_name.substr(0, output_file_name.size() - 4) + ".txt";
  ofstream txtOutputFile(txtOutputPath);
  vector<Section *> sections = secTable->getAllSections();
  int cnt, addr;
  Section *sec;
  vector<char> data;
  vector<char> tmpData;
  for (auto it = sections.begin(); it != sections.end(); it++){
    sec = *it;
    for (int i = 0; i < sec->getData().size(); i++)
    {
      tmpData = sec->getDataPart(i);
      data.insert(data.end(), tmpData.begin(), tmpData.end());
    }
    cnt = 0;
    addr = sec->getAddress();   
    for (auto it = data.begin(); it != data.end(); it++){
      if (cnt % 8 == 0){
        txtOutputFile << setfill('0') << setw(8) << hex << addr+cnt << ":";
      }

      txtOutputFile << " " << hex << setfill('0') << setw(2) << ((int)(*it) & 0x000000ff);
      cnt++;

      if (cnt % 8 == 0){
        txtOutputFile << endl;
      }
    }
    if (cnt % 8 != 0) txtOutputFile << endl;
    
  }
  txtOutputFile << endl;
}

void Linker::relOutputTxt(){
  string txtOutputPath = output_file_name.substr(0, output_file_name.size() - 2) + ".txt";
  ofstream txtOutputFile(txtOutputPath);

  txtOutputFile << "Relocatable object file" << endl
                << endl;

  // Symbol Table
  txtOutputFile << "Symbol Table" << endl;
  txtOutputFile << "Num  Value    Size   Type  Bind  Ndx  Name" << endl;
  vector<Symbol *> symbols = symbolTable->getAllSymbols();
  for (auto it = symbols.begin(); it != symbols.end(); it++)
  {
    Symbol *s = *it;
    if (s->getId() == 0)
      continue; // abs se ne izvozi
    // if(!s->isGlobal() && !s->isSection()) continue; // izvoze se samo globalni simboli i sekcije
    string bind = (s->isGlobal()) ? "GLOB" : "LOC";
    string ndx;
    if (s->getSection() == 0)
      ndx = "ABS";
    else if (s->getSection() == 65535)
      ndx = "UND";
    else
      ndx = to_string(s->getSection());
    txtOutputFile << s->getId() << ":  "
                  << " " << setfill('0') << setw(8) << hex << s->getValue()
                  << " " << setfill(' ') << setw(4) << s->getSize() << "  " << setw(5) << s->getType() << "  "
                  << setw(4) << bind << "  " << setw(3) << ndx << "  " << s->getName() << endl;
  }
  txtOutputFile << endl;

  // Sections
  vector<Section *> sections = secTable->getAllSections();
  for (auto it = sections.begin(); it != sections.end(); it++)
  {
    Section *sec = *it;
    if (sec->getSectionSymbol()->getName() == "ABS")
      continue;        // za ABS
    vector<char> data; // = sec->getData();
    vector<char> tmpData;
    for (int i = 0; i < sec->getData().size(); i++)
    {
      tmpData = sec->getDataPart(i);
      data.insert(data.end(), tmpData.begin(), tmpData.end());
    }

    txtOutputFile << "." << sec->getSectionSymbol()->getName() << endl;
    int cnt = 0;
    for (auto it = data.begin(); it != data.end(); it++)
    {
      txtOutputFile << hex << setfill('0') << setw(2) << ((int)(*it) & 0x000000ff) << " ";
      cnt++;
      if (cnt == 8)
      {
        txtOutputFile << endl;
        cnt = 0;
      }
    }
    if (cnt != 0)
      txtOutputFile << endl;
  }
  txtOutputFile << endl;

  // Relocation Tables
  vector<RelocationTable *> allRelTables = relTables; // privremena lista svih relokacionih tabela iz svih fajlova
  for (auto it = allRelTables.begin(); it != allRelTables.end();)
  {
    RelocationTable *relTable = *it;
    if (relTable->getSection()->getId() == 1)
      continue; // za ABS
    string secName = relTable->getSection()->getName();
    vector<RelocationTable *> simRelTables = getSimilarRelTables(secName);
    txtOutputFile << ".rela." << relTable->getSection()->getName() << endl;
    txtOutputFile << "Offset"
                  << "    "
                  << "Type"
                  << "              "
                  << "Symbol"
                  << "   "
                  << "Addend" << endl;
    for (auto it2 = simRelTables.begin(); it2 != simRelTables.end(); it2++)
    {
      relTable = *it2;
      if (relTable->size() == 0)
        continue;
      vector<Relocation *> relocations = relTable->getRelocations();
      for (auto it = relocations.begin(); it != relocations.end(); it++)
      {
        Relocation *r = *it;
        txtOutputFile << hex << setfill('0') << setw(8) << r->offset << "  " << setfill(' ') << setw(12)
                      << r->type << "  " << setw(8) << r->symbol << "  " << setw(4) << r->addend << endl;
      }
    }
    // brisemo sve obidjene rel tabele iz privremene liste
    for (auto it = allRelTables.begin(); it != allRelTables.end();)
    {
      relTable = *it;
      if (relTable->getSection()->getName() == secName)
      {
        it = allRelTables.erase(it);
      }
      else
      {
        it++;
      }
    }
    if (allRelTables.empty())
      break;
    it = allRelTables.begin();
  }
}

vector<RelocationTable *> Linker::getSimilarRelTables(string secName)
{
  vector<RelocationTable *> retvec;
  RelocationTable *rt;
  for (auto it = relTables.begin(); it != relTables.end(); it++)
  {
    rt = *it;
    if (rt->getSection()->getName() == secName)
    {
      retvec.push_back(rt);
    }
  }
  return retvec;
}

void Linker::relOutputBin()
{
  string binOutputPath = output_file_name;
  ofstream binOutputFile(binOutputPath, ios::out | ios::binary);
  int curr_offset, prev_offset;

  // ELF header
  uint32_t prog_header_offset = 0;                                        // 4B
  uint32_t sec_header_offset = 24;                                        // 4B
  uint32_t flags = 1;                                                     // 1->REL, 0->EXE                                           // 4B
  uint16_t elf_header_size = 24;                                          // 2B
  uint16_t prog_header_entry_size = 0;                                    // 2B
  uint16_t prog_header_num_entries = 0;                                   // 2B
  uint16_t sec_header_entry_size = 24;                                    // 2B
  uint16_t sec_header_num_entries = secTable->size() + numRels() + 1 + 1; // 2B   // 1 zbog Tabele simbola +1 zbog Tabele Stringovas
  uint16_t string_section_index = sec_header_num_entries - 1;             // 2B

  binOutputFile.write((char *)(&prog_header_offset), sizeof(prog_header_offset));
  binOutputFile.write((char *)(&sec_header_offset), sizeof(sec_header_offset));
  binOutputFile.write((char *)(&flags), sizeof(flags));
  binOutputFile.write((char *)(&elf_header_size), sizeof(elf_header_size));
  binOutputFile.write((char *)(&prog_header_entry_size), sizeof(prog_header_entry_size));
  binOutputFile.write((char *)(&prog_header_num_entries), sizeof(prog_header_num_entries));
  binOutputFile.write((char *)(&sec_header_entry_size), sizeof(sec_header_entry_size));
  binOutputFile.write((char *)(&sec_header_num_entries), sizeof(sec_header_num_entries));
  binOutputFile.write((char *)(&string_section_index), sizeof(string_section_index));

  // Section header table
  string names = "0";
  int curr_sh = 0;
  int sh_strndx_offs = 0, sh_type_off = 2, sh_addr_offs = 4, sh_offs_offs = 8;
  int sh_size_offs = 12, sh_link_offs = 16, sh_info_off = 18, sh_entsize_offs = 20;
  uint16_t sh_strndx = 0x0;  // 2B
  uint16_t sh_type = 0x0;    // 2B
  uint32_t sh_addr = 0x0;    // 4B
  uint32_t sh_offs = 0x0;    // 4B
  uint32_t sh_size = 0x0;    // 4B
  uint16_t sh_link = 0x0;    // 2B
  uint16_t sh_info = 0x0;    // 2B
  uint32_t sh_entsize = 0x0; // 4B

  for (int i = 0; i < sec_header_num_entries; i++)
  {
    binOutputFile.write((char *)(&sh_strndx), sizeof(sh_strndx));
    binOutputFile.write((char *)(&sh_type), sizeof(sh_type));
    binOutputFile.write((char *)(&sh_addr), sizeof(sh_addr));
    binOutputFile.write((char *)(&sh_offs), sizeof(sh_offs));
    binOutputFile.write((char *)(&sh_size), sizeof(sh_size));
    binOutputFile.write((char *)(&sh_link), sizeof(sh_link));
    binOutputFile.write((char *)(&sh_info), sizeof(sh_info));
    binOutputFile.write((char *)(&sh_entsize), sizeof(sh_entsize));
  }

  // SECTIONS
  curr_sh = 0;
  curr_offset = sec_header_offset + sec_header_entry_size * sec_header_num_entries;
  prev_offset = 0;
  uint32_t st_name;
  uint8_t st_bind;
  uint8_t st_type;
  uint16_t st_secndx; // r.b. u section_header = r.b. tabeli sekcija (-1 zbog abs, +1 zbog tabele simbola)
  uint32_t st_value;
  // Symbol Table
  int sym_tab_sh_entry = curr_sh;
  int cnt_exported = 0;
  for (Symbol *s : symbolTable->getAllSymbols())
  {
    st_name = names.size();
    names += s->getName() + "0";
    st_bind = s->isGlobal() ? 1 : 0;
    st_type = s->isSection() ? 3 : 0;
    if (s->getSection() == 0 || s->getSection() == 65535 || s->getSection() == -1)
      st_secndx = s->getSection();
    else
      st_secndx = s->getSection();

    st_value = s->getValue();

    binOutputFile.write((char *)(&st_name), sizeof(st_name));
    binOutputFile.write((char *)(&st_bind), sizeof(st_bind));
    binOutputFile.write((char *)(&st_type), sizeof(st_type));
    binOutputFile.write((char *)(&st_secndx), sizeof(st_secndx));
    binOutputFile.write((char *)(&st_value), sizeof(st_value));
    cnt_exported++;
  }
  prev_offset = curr_offset;
  curr_offset = binOutputFile.tellp();

  // upisi sve kako treba u section header
  sh_strndx = names.size();
  names += "symtab0";
  sh_type = 0x2;
  sh_addr = 0x0;
  sh_offs = prev_offset;
  sh_size = curr_offset - prev_offset;
  sh_link = string_section_index;
  sh_info = cnt_exported;
  sh_entsize = 0xC;
  binOutputFile.seekp(sec_header_offset + curr_sh * sec_header_entry_size);
  binOutputFile.write((char *)(&sh_strndx), sizeof(sh_strndx));
  binOutputFile.write((char *)(&sh_type), sizeof(sh_type));
  binOutputFile.write((char *)(&sh_addr), sizeof(sh_addr));
  binOutputFile.write((char *)(&sh_offs), sizeof(sh_offs));
  binOutputFile.write((char *)(&sh_size), sizeof(sh_size));
  binOutputFile.write((char *)(&sh_link), sizeof(sh_link));
  binOutputFile.write((char *)(&sh_info), sizeof(sh_info));
  binOutputFile.write((char *)(&sh_entsize), sizeof(sh_entsize));
  binOutputFile.seekp(curr_offset);

  // Regular Sections
  for (Section *sec : secTable->getAllSections())
  {
    if (sec->getSectionSymbol()->getName() == "ABS")
      continue; // za ABS
    curr_sh++;

    for (int i = 0; i < sec->getData().size(); i++)
    {
      vector<char> data = sec->getDataPart(i);
      binOutputFile.write((char *)(&data[0]), data.size());
    }

    // allignment to 4
    int rem, tmp = 0;
    rem = binOutputFile.tellp() % 4;
    if (rem > 0)
      binOutputFile.write((char *)(&tmp), 4 - rem);

    prev_offset = curr_offset;
    curr_offset = binOutputFile.tellp();

    // upisi sve kako treba u section header
    sh_strndx = names.find(sec->getSectionSymbol()->getName());
    sh_type = 0x1;
    sh_addr = 0x0;
    sh_offs = prev_offset;
    sh_size = curr_offset - prev_offset;
    if (rem > 0)
      sh_size -= (4 - rem); // ne racunamo padding
    sh_link = 0x0;
    sh_info = 0x0;
    sh_entsize = 0x0;
    binOutputFile.seekp(sec_header_offset + curr_sh * sec_header_entry_size);
    binOutputFile.write((char *)(&sh_strndx), sizeof(sh_strndx));
    binOutputFile.write((char *)(&sh_type), sizeof(sh_type));
    binOutputFile.write((char *)(&sh_addr), sizeof(sh_addr));
    binOutputFile.write((char *)(&sh_offs), sizeof(sh_offs));
    binOutputFile.write((char *)(&sh_size), sizeof(sh_size));
    binOutputFile.write((char *)(&sh_link), sizeof(sh_link));
    binOutputFile.write((char *)(&sh_info), sizeof(sh_info));
    binOutputFile.write((char *)(&sh_entsize), sizeof(sh_entsize));
    binOutputFile.seekp(curr_offset);
  }

  // Relocation Tables
  uint32_t r_offset;
  uint32_t r_info_symbol;
  uint32_t r_addend;
  vector<RelocationTable *> allRelTables = relTables; // privremena lista svih relokacionih tabela iz svih fajlova
  for (auto it = allRelTables.begin(); it != allRelTables.end();)
  {
    RelocationTable *relTable = *it;
    if (relTable->getSection()->getId() == 1)
      continue; // za ABS
    string secName = relTable->getSection()->getName();
    vector<RelocationTable *> simRelTables = getSimilarRelTables(secName);
    // INKREMENTIRA SE ULAZ U SECTION HEADER ZA SVAKI SKUP REL TABELA ZA ISTU SEKCIJU
    curr_sh++;
    for (auto it2 = simRelTables.begin(); it2 != simRelTables.end(); it2++)
    {
      RelocationTable *relTable2 = *it2;
      if (relTable2->size() == 0)
        continue;
      vector<Relocation *> relocations = relTable2->getRelocations();
      for (auto it = relocations.begin(); it != relocations.end(); it++)
      {
        Relocation *r = *it;
        // OVDE IDE OBRADA ZA SVAKU RELOKACIJU
        r_offset = r->offset;
        r_info_symbol = (r->type == RelocationType::R_X86_64_32) ? 11 : 2; // ABS->11, PC->2
        r_info_symbol <<= 16;
        r_info_symbol |= (uint32_t)symbolTable->findSymbol(r->symbol)->getId() - 1; // zbog ABS
        r_addend = r->addend;

        binOutputFile.write((char *)(&r_offset), sizeof(r_offset));
        binOutputFile.write((char *)(&r_info_symbol), sizeof(r_info_symbol));
        binOutputFile.write((char *)(&r_addend), sizeof(r_addend));
        // KRAJ
      }
    }
    // OVDE IDE OBRADA ZA SVAKU TABELU RELOKACIJA
    prev_offset = curr_offset;
    curr_offset = binOutputFile.tellp();

    // upisi sve kako treba u section header
    sh_strndx = names.size();
    names += ".rela." + relTable->getSection()->getName() + "0";
    sh_type = 0x4;
    sh_addr = 0x0;
    sh_offs = prev_offset;
    sh_size = curr_offset - prev_offset;
    sh_link = sym_tab_sh_entry;
    sh_info = secTable->findSectionId(relTable->getSection());
    sh_entsize = sizeof(r_offset) + sizeof(r_info_symbol) + sizeof(r_addend);
    binOutputFile.seekp(sec_header_offset + curr_sh * sec_header_entry_size);
    binOutputFile.write((char *)(&sh_strndx), sizeof(sh_strndx));
    binOutputFile.write((char *)(&sh_type), sizeof(sh_type));
    binOutputFile.write((char *)(&sh_addr), sizeof(sh_addr));
    binOutputFile.write((char *)(&sh_offs), sizeof(sh_offs));
    binOutputFile.write((char *)(&sh_size), sizeof(sh_size));
    binOutputFile.write((char *)(&sh_link), sizeof(sh_link));
    binOutputFile.write((char *)(&sh_info), sizeof(sh_info));
    binOutputFile.write((char *)(&sh_entsize), sizeof(sh_entsize));
    binOutputFile.seekp(curr_offset);
    // KRAJ

    for (auto it = allRelTables.begin(); it != allRelTables.end();)
    {
      relTable = *it;
      if (relTable->getSection()->getName() == secName)
      {
        it = allRelTables.erase(it);
      }
      else
      {
        it++;
      }
    }
    if (allRelTables.empty())
      break;
    it = allRelTables.begin();
  }

  // Tabela Stringova
  curr_sh++;

  int str_tab_ndx = names.size();
  names += ".strtab0";
  binOutputFile.write(names.c_str(), names.size());
  // allignment to 4
  int rem, tmp = 0;
  rem = binOutputFile.tellp() % 4;
  if (rem > 0)
    binOutputFile.write((char *)(&tmp), 4 - rem);

  prev_offset = curr_offset;
  curr_offset = binOutputFile.tellp();

  // upisi sve kako treba u section header
  sh_strndx = str_tab_ndx;
  sh_type = 0x3;
  sh_addr = 0x0;
  sh_offs = prev_offset;
  sh_size = curr_offset - prev_offset;
  sh_link = 0x0;
  sh_info = 0x0;
  sh_entsize = 0x0;
  binOutputFile.seekp(sec_header_offset + curr_sh * sec_header_entry_size);
  binOutputFile.write((char *)(&sh_strndx), sizeof(sh_strndx));
  binOutputFile.write((char *)(&sh_type), sizeof(sh_type));
  binOutputFile.write((char *)(&sh_addr), sizeof(sh_addr));
  binOutputFile.write((char *)(&sh_offs), sizeof(sh_offs));
  binOutputFile.write((char *)(&sh_size), sizeof(sh_size));
  binOutputFile.write((char *)(&sh_link), sizeof(sh_link));
  binOutputFile.write((char *)(&sh_info), sizeof(sh_info));
  binOutputFile.write((char *)(&sh_entsize), sizeof(sh_entsize));
  binOutputFile.seekp(curr_offset);
}

int Linker::numRels()
{
  set<string> names;
  int cnt = 0;
  for (RelocationTable *r : relTables)
  {
    names.insert(r->getSection()->getName());
  }
  return names.size();
}