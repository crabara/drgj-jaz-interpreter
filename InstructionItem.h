/* 
 * File:   InstructionItem.h
 * Author: Jag
 *
 * Created on February 11, 2012, 6:20 AM
 */

#ifndef INSTRUCTIONITEM_H
#define	INSTRUCTIONITEM_H

#include <string>

using namespace std;

//A pretty basic linked list architecture.  I chose a LL because it's the
//easiest way to move back and forth in threads of execution, and I can jump in
//anywhere in the list.  Scope will be easier to maintain once implemented.
class InstructionItem {
    
public:
    InstructionItem();
    InstructionItem(string tempCommand, string tempArg);
    virtual ~InstructionItem();
    
    void setCommand(string tempCommand);
    void setArg(string tempArg);
    void setNext(InstructionItem* tempNextPtr);
    
    string getCommand();
    string getArg();
    InstructionItem* getNext();
    
private:
    string command;
    string arg;
    InstructionItem* next;

};

#endif	/* INSTRUCTIONITEM_H */
