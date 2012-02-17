#ifndef INSTRUCTIONITEM_H
#define	INSTRUCTIONITEM_H

#include <string>

using namespace std;

class InstructionItem {
    
public:
    InstructionItem();
    InstructionItem(string tempCommand, string tempArg, int tempLineNumber);
    virtual ~InstructionItem();
    
    void setLineNumber(int tempLineNumber);
    void incrementAccess();
    void setCommand(string tempCommand);
    void setArg(string tempArg);
    void setNext(InstructionItem* tempNextPtr);
    
    int getLineNumber();
    int getAccess();
    string getCommand();
    string getArg();
    InstructionItem* getNext();
    
private:
    int lineNumber;
    int accessAttempt;
    string command;
    string arg;
    InstructionItem* next;

};

#endif	/* INSTRUCTIONITEM_H */
