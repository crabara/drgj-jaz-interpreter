#ifndef EXECUTER_H
#define	EXECUTER_H

#include "InstructionItem.h"
#include "SymbolTable.h"
#include <iostream>
#include <fstream>
#include <stack>

//Executer handles all the execution of the jaz file. It loops continuously,
//performing actions based on the current command until a halt command is
//reached.
class Executer {
public:
    Executer();
    virtual ~Executer();
    
    void setInstructionList(InstructionItem* listHead);
    void setLabelMap(map<string, InstructionItem*> labelMap);
    
    InstructionItem* getCurrentInstruction();
    map<string, InstructionItem*> getLabelMap();
    
    void execute(ofstream& os, bool fileIsOpen = true);
    
private:
    //Points to the current instruction being executed.
    InstructionItem* instrList;
    //Holds the labels and associates them with the instruction they point to.
    map<string, InstructionItem*> labelMap;
    //The stack that represents the memory of this machine.
    stack<string> memoryStack;
    //symbolTables holds the variable lists and changes as the scope of the
    //program changes.  As new subroutines are started, the current variable
    //list is pushed back on the stack, then popped off as subroutines end.
    stack<SymbolTable*> symbolTables;
    //These pointers handle the current scope of the program, wideScope handles
    //variable assigns (:=) while narrowScope handles variable reads (rvalue).
    SymbolTable* wideScope;
    SymbolTable* narrowScope;
    //returnToCaller allows the call and return commands to work by pushing the
    //location of the line that should be next onto this stack then popping it
    //off when a return is performed.
    stack<InstructionItem*> returnToCaller;
    
    string popTop();
    string convertInt(int intToConvert);
    void errMessage(int lineNumber, int access, string problem);
    void haltMessage(string command);

};

#endif	/* EXECUTER_H */
