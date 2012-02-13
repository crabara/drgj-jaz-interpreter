/* 
 * File:   Reader.h
 * Author: Jag
 *
 * Created on February 11, 2012, 11:44 PM
 */

#ifndef READER_H
#define	READER_H

#include "InstructionItem.h"
#include <iostream>
#include <fstream>
#include <map>

//Reader handles all the parsing functions of the program.  It converts the base
//.jaz file into a linked list of executions, maps the labels to the appropriate
//place to start executing when called.
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
