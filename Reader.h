/* 
 * File:   Reader.h
 * Author: Jag
 *
 * Created on February 11, 2012, 11:44 PM
 */

#ifndef READER_H
#define	READER_H

#include "InstructionItem.h"
#include <fstream>
#include <map>

using namespace std;

//Reader handles all the parsing functions of the program.  It converts the base
//.jaz file into a linked list of executions, maps the labels to the appropriate
//place to start executing when called.
class Reader {
public:
    Reader();
    virtual ~Reader();
    
    static pair<string, string> parseLine(string line);
    static InstructionItem* buildList(ifstream& is);
    static map<string, InstructionItem*> mapLabels(InstructionItem* listItem);
    
private:

};

#endif	/* READER_H */

