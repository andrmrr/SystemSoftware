#include <iostream>
#include "linker.h"

using namespace std;

int main(int argc, char** argv){
  cout << "Linker" << endl;

  vector<string> inputFiles;
  vector<string> places;
  string outputFile = "";
  bool error = false;
  bool linkTypeDefined = false;
  bool outputFileDefined = false;
  bool isHex = false;
  char* arg = (char*)calloc(100, sizeof(char));
  char* argTMP = arg;
  try{
    for(int i = 1; i < argc; i++){
      strcpy(arg, argv[i]);
      if(strcmp(arg, "-o") == 0){
        if(outputFileDefined == false){
          outputFileDefined = true;
          outputFile = string(argv[++i]);
        } else {
          cout << "Naziv izlaznog fajla se moze navesti samo jednom!" << endl;
          error = true;
          break;
        }
      } else if(strcmp(arg, "-hex") == 0){
        if(linkTypeDefined == false){
          linkTypeDefined = true;
          isHex = true;
        } else {
          cout << "Tip povezivanja se moze navesti samo jednom!" << endl;
          error = true;
          break;
        }
      } else if(strcmp(arg, "-relocatable") == 0){
        if(linkTypeDefined == false){
          linkTypeDefined = true;
          isHex = false;
        } else {
          cout << "Tip povezivanja se moze navesti samo jednom!" << endl;
          error = true;
          break;
        }
      } else if(strstr(arg, "-place=") != nullptr){
        arg += 7;
        places.push_back(string(arg));
      } else {
        inputFiles.push_back(string(arg));
      }
    }
  } catch(exception e){
    free(argTMP);
    cout << e.what();
    return 1;
  }
  free(argTMP);

  if(!linkTypeDefined || inputFiles.empty()) {
    return 0;
  }

  //Izvrsavanje linkera
  if(!error){
    try{

      Linker ld(inputFiles, isHex, places, outputFile);
      ld.link();

    } catch(const SymbolError& se){
      cout << se.whatSymbol();
    } catch(exception e){
      cout << e.what();
    }
    return 0;
  } else {
    return 2;
  }
}