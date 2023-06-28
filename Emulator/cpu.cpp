#include "cpu.h"

CPU::CPU(){

}

CPU::~CPU(){

}

void CPU::loadMemory(string ifName){
  ifstream inputFile(ifName);
  char* charr = (char*)calloc(100, sizeof(char));
  string line, saddr;
  vector<string> parts;
  uint32_t addr;

  inputFile.getline(charr, 100);   
  while(!inputFile.eof()){
    line = string(charr);
    parts = split(line, ' ');
    if(parts.empty()) break;

    saddr = parts.front();
    addr = hexStringToUInt32(saddr.substr(0, saddr.size()-1), false);
    parts.erase(parts.begin());
    for(auto it = parts.begin(); it != parts.end(); it++){
      writeByte(addr++, hexStringToUInt32((*it), false));
    }
    inputFile.getline(charr, 100);
  }

  free(charr);
  for(auto it = memory.begin(); it != memory.end(); it++){
      cout << it->first << " " << (int)it->second << endl;
    }
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

void CPU::writeByte(uint32_t addr, Byte b){
  memory.insert(pair<uint32_t, Byte>(addr, b));
}

Byte CPU::readByte(uint32_t addr){
  return memory[addr]; 
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