#include <vector>
#include <iostream>
#include <fstream>
#include "tokens.h"
#include "lex.yy.h"
#include "asembler.h"

using namespace std;

int main(int argc, char** argv){
  if(argc != 2 && argc != 4) {
    cout << "Sintaksa pokretanja asemblera:\nasembler [-o <naziv_izlazne_datoteke>] <naziv_ulazne_datoteke>\n";
    return 1;
  }

  string outf;
  vector<Token> tokens;
  bool parsingError = false;
 

  //citanje iz ulaznog fajla
  try{
    ifstream input_file;
    if(argc == 2){
      input_file = ifstream(argv[1]);
      outf = string(argv[1]);
      outf = outf.substr(0, outf.size()-2);
    } else {
      if(strlen(argv[1]) != 2 || argv[1][0] != '-' || argv[1][1] != 'o'){
        cout << "Sintaksa pokretanja asemblera:\nasembler [-o <naziv_izlazne_datoteke>] <naziv_ulazne_datoteke>\n";
        return 1;
      }
      outf = string(argv[2]);
      outf = outf.substr(0, outf.size()-2);
      input_file = ifstream(argv[3]);
    }

    Lexer lexer(input_file);
    Token token;
    int lineCounter = 1;

    token = lexer.lex();
    // cout << "Tokens:" << endl; 
    while(token.getType() != TokenType::END_FILE){
      // cout << token.getType() << " ";
      // cout << "Token " << token.getType() << " " << token.getText() << endl;
      tokens.push_back(token);
      if(token.getType() == TokenType::EOL) {
        lineCounter++;
        // cout << endl;
      }
      if(token.getType() == TokenType::ERROR){
        cout << "GRESKA PRI PARSIRANJU NA LINIJI " << lineCounter << endl;
        parsingError = true;
        break;
      }
      token = lexer.lex();
    }
  } catch(...){
    cout << "Greska u radu sa fajlovima.\n";
    return 2;
  }

  if(!parsingError) {
    try{
      Asembler as = Asembler(tokens, outf);
      // cout << "dosli do prvog prolaza" << endl;
      as.firstPass();
      // cout << "zavrsili prvi prolaz" << endl;
      if(!as.getError()){
        // cout << "zapoceli drugi prolaz" << endl;
        as.secondPass();
        // cout << "zavrsili drugi prolaz" << endl;
        as.createTextFile();
        as.createBinaryFile();
      }
    } catch(AlreadyDefinedException adex) {
      cout << adex.whatSymbol() << endl;
      return 0;
    } catch(...) {
      cout << "Pakao" << endl;
      return 3;
    }
  }

  return 0;
}