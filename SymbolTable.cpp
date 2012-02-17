#include "SymbolTable.h"

SymbolTable::SymbolTable() {
}

SymbolTable::~SymbolTable() {
}

void SymbolTable::setVariable(string key, string value) {
    this->variableMap[key] = value;
}

string SymbolTable::getVariable(string key) {
    return this->variableMap[key];
}
