#ifndef SYMBOLTABLE_H
#define	SYMBOLTABLE_H

#include <string>
#include <map>

using namespace std;

//SymbolTable is simply a wrapper for variable maps.  It provides simple
//accessors to make working with lvalue and rvalue simpler.
class SymbolTable {
public:
    SymbolTable();
    virtual ~SymbolTable();
    
    void setVariable(string key, string value);
    string getVariable(string key);
    
private:
    map<string, string> variableMap;

};

#endif	/* SYMBOLTABLE_H */
