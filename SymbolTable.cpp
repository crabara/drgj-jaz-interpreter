#include "SymbolTable.h"

SymbolTable::SymbolTable() { }

SymbolTable::~SymbolTable() { }

//sets a variable into the table by creating or overwriting a key (lvalue) with
//a value (rvalue).
void SymbolTable::setVariable(string key, string value) {
    this->variableMap[key] = value;
}

//resolves a rvalue by using its lvalue as a key to the map.
string SymbolTable::getVariable(string key) {
    return this->variableMap[key];
}
