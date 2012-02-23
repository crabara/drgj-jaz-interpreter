#ifndef INSTRUCTIONITEM_H
#define	INSTRUCTIONITEM_H

#include <string>

using namespace std;

//Instruction item is a simple linked list of objects.  Each object contains
//the command and argument from a single line in a jaz file.  The other
//variables are used for error control and reporting.
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
    //number of the line on which this instruction is written in the jaz file.
    int lineNumber;
    //this holds how many times this line was accessed in order to provide more
    //robust error tracking to the jaz file writer.
    int accessAttempt;
    //the command of the given line.
    string command;
    //the argument of a given line.
    string arg;
    //a pointer to the next instruction.
    InstructionItem* next;

};

#endif	/* INSTRUCTIONITEM_H */
