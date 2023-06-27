c++ -I./modules -g -o linker  \
main.cpp linker.cpp linker_output.cpp \
modules/sectionTable.cpp modules/section.cpp modules/relocationTable.cpp modules/symbolTable.cpp modules/symbol.cpp