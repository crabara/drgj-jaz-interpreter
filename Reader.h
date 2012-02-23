#ifndef READER_H
#define	READER_H

#include "InstructionItem.h"
#include <fstream>

//Reader iterates through the jaz file line by line and builds a linked list of
//instructions, returning a pointer to the first instruction to the caller.
class Reader {
public:
    Reader();
    virtual ~Reader();
    
    InstructionItem* buildList(ifstream& is);
    
private:
    //lineTracker increments each time buildList is called, it tracks what line
    //the current instruction appears on in the jaz file.
    int lineTracker;
    
    pair<string, string> parseLine(string line);

};

#endif	/* READER_H */
