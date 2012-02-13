#ifndef EXECUTER_H
#define	EXECUTER_H

#include "InstructionItem.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <map>

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
    InstructionItem* instrList;
    map<string, InstructionItem*> labelMap;
    stack<string> memoryStack;
    map<string, string> variableList;
    stack<InstructionItem*> returnToCaller;
    
    pair<string, string> popTwo();
    string convertInt(int intToConvert);

};

#endif	/* EXECUTER_H */
