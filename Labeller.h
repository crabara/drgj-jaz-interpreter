#ifndef LABELLER_H
#define	LABELLER_H

#include "InstructionItem.h"
#include <iostream>
#include <map>

//labeller simply iterates through the linked list of instructions looking for
//label commands and returns a map that resolves each label argument to a
//pointer telling any callers of that label where to pick up execution.
class Labeller {
public:
    Labeller();
    virtual ~Labeller();
    
    map<string, InstructionItem*> mapLabels(InstructionItem* listHead);
    
private:

};

#endif	/* LABELLER_H */
