#include <vector>
#include <iostream>
#include <fstream>
#include "tokens.h"
#include "lex.yy.h"
#include "asembler.h"

using namespace std;

int main(int argc, char** argv){

  ifstream file(argv[1]);

  Lexer lexer(file);
  vector<Token> tokens;
  Token token;

  int lineCounter = 1;
  bool parsingError = false;

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

  if(!parsingError) {
    Asembler as = Asembler(tokens);
    cout << "dosli do prvog prolaza" << endl;
    as.firstPass();
    cout << "zavrsili prvi prolaz" << endl;
    if(!as.getError()){
      cout << "zapoceli drugi prolaz" << endl;
      as.secondPass();
      cout << "zavrsili drugi prolaz" << endl;
    }
  }

}