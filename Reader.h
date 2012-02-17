#ifndef READER_H
#define	READER_H

#include "InstructionItem.h"
#include <iostream>
#include <fstream>
#include <map>

class Reader {
public:
    Reader();
    virtual ~Reader();
    
    InstructionItem* buildList(ifstream& is);
    map<string, InstructionItem*> mapLabels(InstructionItem* listItem);
    
private:
    int lineTracker;
    
    pair<string, string> parseLine(string line);

};

#endif	/* READER_H */
