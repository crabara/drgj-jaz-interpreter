#ifndef SYMBOLTABLE_H
#define	SYMBOLTABLE_H

#include <string>
#include <map>

using namespace std;

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
