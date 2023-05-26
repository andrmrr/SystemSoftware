#include <list>
#include <iostream>
#include <fstream>
#include "tokens.h"
#include "lex.yy.h"

using namespace std;

int main(){
  vector<Token> tokens;

  ifstream file("example.txt");

  Lexer lexer(file);
  Token token;

  token = lexer.lex();
  while(token.getType() != TokenType::END_FILE){
      tokens.push_back(token);
      //cout << "Token " << token.getType() << " " << token.getText() << endl;

      token = lexer.lex();
  }

}