#include "cpu.h"

CPU::CPU(){
  reset();
}

CPU::~CPU(){

}

void CPU::reset(){
  gpr[15] = 0x40000000;
  gpr[14] = 0xF0000000;
  memory = map<uint32_t, Word>();
  state = CPU::finished;
}

void CPU::loadMemory(string ifName){
  ifstream inputFile(ifName);
  char* charr = (char*)calloc(100, sizeof(char));
  vector<string> parts;
  string line, saddr;
  uint32_t addr;
  Word data;

  inputFile.getline(charr, 100);   
  while(!inputFile.eof()){
    line = string(charr);
    parts = split(line, ' ');
    if(parts.empty()) break;

    saddr = parts.front();
    addr = hexStringToUInt32(saddr.substr(0, saddr.size()-1), false);
    parts.erase(parts.begin());

    if(parts.size() % 4 != 0){
      for(int i = parts.size(); i % 8 != 0; i ++){
        parts.push_back("00");
      }
    }

    data = 0;
    for(int i = 0; i < parts.size(); i++){
      int pos = 4 - (i % 4) - 1; 
      data |=  hexStringToUInt32((parts[i]), false) << (pos*8);
      if(i > 0 && i % 4 == 3){
        writeWord(addr, data);
        addr += 4;
        data = 0;
      }
    }
    inputFile.getline(charr, 100);
  }

  free(charr);
  // ofstream log("log.txt");
  // for(auto it = memory.begin(); it != memory.end(); it++){
  //     log << setfill('0') << setw(8) << hex << it->first << " " << setw(8) << (int)it->second << endl;
  // }
}

vector<string> CPU::split(const string line, char delim) {
  vector<string> tokens;
  string token;
  istringstream tokenStream(line);

  while (getline(tokenStream, token, delim)) {
    tokens.push_back(token);
  }

  return tokens;
}

void CPU::writeWord(uint32_t addr, Word w){
  memory[addr] = w;
}

Word CPU::readWord(uint32_t addr){
  return memory[addr]; 
}

void CPU::print(){
  cout << "--------------------------------------------------------------------" << endl;
  cout << "Emulated processor executed halt instruction" << endl;
  cout << "Emulated processor state:" << endl;
  for(int i = 0; i < 4; i ++){
    for(int j = 0; j < 4; j ++){
      int pos = i*4 + j;
      if(pos <= 9) cout << " ";
      cout << "r" << dec << pos << "=0x" << setfill('0') << setw(8) << hex << gpr[pos] << "    ";
    }
    cout << endl;
  }
}

uint32_t CPU::hexStringToUInt32(string hex, bool prefix){
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