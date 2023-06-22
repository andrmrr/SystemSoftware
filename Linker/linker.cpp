#include "linker.h"


Linker::Linker(vector<string> ifnames) {
  this->input_file_names = ifnames;
  //outputFilePath = outf;
  //error = false;
  //lcounter = 0;
  //currSection = nullptr;
  SymbolTable::initSymbolTable();
  symbolTable = SymbolTable::getInstance();


  SectionTable::initSectionTable();
  secTable = SectionTable::getInstance();
  vector<Symbol*> secSymbols = symbolTable->getSections();
  // for(auto it = secSymbols.begin(); it != secSymbols.end(); it++){
  //   secTable->addSection(*it);
  //   relTables.push_back(new RelocationTable(*it));
  // }
}

Linker::~Linker() {
  
}

void Linker::link(){
  solveReferences();
  solveRelocations();
}

void Linker::solveReferences(){
  for(string ifname: input_file_names){
    ifstream inputBinFile(ifname, ios_base::in | ios_base::binary);
    int curr_offset, prev_offset;

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

    curr_offset = sec_header_offset + 0*sec_header_entry_size;

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
    string name;
    Symbol* s;
    char* nameTemp = (char*)malloc(100*sizeof(char));
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
      if(st_type == 3 || st_type == 1) {
        symbolTable->addSection(name, ifname);
      } else {
        s = symbolTable->addSymbol(name, st_secndx, st_value, ifname);
        if(st_bind == 1) s->setGlobal();
      }
    }
    free(nameTemp);
    cout << "Prosli jedan fajl" << endl;
  }
}

void Linker::solveRelocations() {

}