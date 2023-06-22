#pragma once
#include <iostream>

class BadFileType : public std::exception {
  public:
    BadFileType() {}
    const char* what() const noexcept override {
      return "Only relocatable files can be used for linking!";
    }
};

class InputFileError : public std::exception {
  public:
    InputFileError() {}
    const char* what() const noexcept override {
      return "There is an error in the input file!";
    }
};


// class BadFileType : public exception {
//   string symbol;
//   public:
//     AlreadyDefinedException(string sym): symbol(sym) {}
//     const char* what() const noexcept override {
//       return "The symbol is already defined: ";
//     }
//     string whatSymbol() const noexcept {
//       return this->what() + symbol;
//     }
// };