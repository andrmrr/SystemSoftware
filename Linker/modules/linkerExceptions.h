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

class SymbolError : public exception {
  protected:
    string symbol;
  public:
    SymbolError(string sym): symbol(sym) {}
    virtual const char* what() const noexcept {
      return "A symbol error occured!\n";
    }
    virtual string whatSymbol() const noexcept = 0;
};

class MultipleDefinitionsError : public SymbolError {
  public:
    MultipleDefinitionsError(string sym) : SymbolError(sym) {}
    const char* what() const noexcept override {
      return "There are multiple definitions of the symbol: ";
    }
    string whatSymbol() const noexcept override {
      return this->what() + symbol + "\n";
    }
};

class UnresolvedSymbolError : public SymbolError {
  public:
    UnresolvedSymbolError(string sym) : SymbolError(sym) {}
    const char* what() const noexcept override {
      return " is not defined!\n";
    }
    string whatSymbol() const noexcept override {
      return "The symbol: " + symbol + this->what();
    }
};

class AddressOverlap : public SymbolError {
  public:
    AddressOverlap(string sym) : SymbolError(sym) {}
    const char* what() const noexcept override {
      return " overlaps with another section!\n";
    }
    string whatSymbol() const noexcept override {
      return "Section: " + symbol + this->what();
    }
};