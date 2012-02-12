/* 
 * File:   Reader.cpp
 * Author: Jag
 * 
 * Created on February 11, 2012, 11:44 PM
 */

#include "Reader.h"

Reader::Reader() {
}

Reader::~Reader() {
}

//rather than stick this (rather ugly) code into the middle of buildList, I put
//it here to be called.  It takes each line of the.jaz file and simply splits it
//into a command and a single argument, removing any whitespace other than
//spaces inside the argument (in order for "show" to work correctly.
pair<string, string> Reader::parseLine(string line) {
    pair<string, string> out;
    
    //white space symbols to ignore.
    string parseSymbols(" \t\r");
    
    size_t commandBegin = line.find_first_not_of(parseSymbols);
    
    if (commandBegin == string::npos) {
        //if the function gets here, the line is blank, send back that this line
        //can be ignored.
        out = make_pair("", "");
        
    } else {
        size_t commandEnd = line.find_first_of(parseSymbols, commandBegin);
        //we've encapsulated our single word command, place it in output.
        out.first = line.substr(commandBegin, commandEnd - commandBegin);
        size_t argBegin = line.find_first_not_of(parseSymbols, commandEnd);
        
        if (argBegin == string::npos) {
            //reaching this place means that there is no argument.  That's fine,
            //any commands requiring an argument will be caught later, in
            //executor.
            out.second = "";
        } else {
            //we've found our full argument, put it in output.
            size_t argEnd = line.find_last_not_of(parseSymbols);
            out.second = line.substr(argBegin, argEnd - argBegin + 1);
        }
    }
    
    return out;
}

//here, we get the filestream from the main program and run it, line by line, to
//build the linked list from which to execute.
InstructionItem* Reader::buildList(ifstream& is) {
    InstructionItem* listItem = NULL;
    //unfortunately, the >> operator for ifstream tokenizes on each space.  This
    //won't work for our purposes, we need to pull in using getline, which means
    //working with those awful c-strings.
    char holder[100];
    is.getline(holder, 100);
    string line(holder);
    
    //now that we have the string we want, let's send it for parsing.
    pair<string, string> instrPair = parseLine(line);
    
    if (instrPair.first.length() == 0) {
        //reaching here means the line was blank, forget about it and move on to
        //the next line (if there is one), otherwise, collapse the recurse into
        //a series of NULL returns until it reaches the last line that wasn't
        //blank
        
        if (!is.eof()) {
            listItem = buildList(is);
        }
    } else {
        //this line is useful, make it a new item at the end of the list.
        listItem = new InstructionItem(instrPair.first, instrPair.second);
        
        if (!is.eof()) {
            //we have more lines to get to, keep moving until the final line,
            //which will have a NULL pointer by default, signaling the end of
            //the list.
            listItem->setNext(buildList(is));
        }
    }
    return listItem;
}

//I found using a map was the only clear choice for labels.  No other STL
//container in C++ has the associative properties necessary to resolve a string
//into a pointer where the label should jump to.
map<string, InstructionItem*> Reader::mapLabels(InstructionItem* listItem) {
    map<string, InstructionItem*> labelList;
    
    while (listItem->getNext() != NULL) {
        if (listItem->getCommand() == "label") {
            labelList[listItem->getArg()] = listItem->getNext();
        }
        listItem = listItem->getNext();
    }
    
    //unfortunately, there's no for_each algorithm for linked lists, so we need
    //to run the logic on the final list item separately.  Ideally, the .jaz
    //file should not end with a label, and it absolutely should not be called,
    //but we'll catch that error, if it comes up, in executor.
    if (listItem->getCommand() == "label") {
        labelList[listItem->getArg()] = listItem->getNext();
    }
    return labelList;
}
