/* 
 * File:   Reader.cpp
 * Author: Jag
 * 
 * Created on February 11, 2012, 11:44 PM
 */

#include "Reader.h"

Reader::Reader() {
    this->lineTracker = 0;
}

Reader::~Reader() {
}

//here, we get the filestream from the main program and run it, line by line, to
//build the linked list from which to execute.
InstructionItem* Reader::buildList(ifstream& is) {
    lineTracker++;
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
        listItem = new InstructionItem(instrPair.first,
                                       instrPair.second,
                                       lineTracker);
        
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
//into a pointer where the label should jump to.  We do this here so that the
//label can be called before the line in which it is defined.
map<string, InstructionItem*> Reader::mapLabels(InstructionItem* listItem) {
    map<string, InstructionItem*> labelList;
    
    while (listItem->getNext() != NULL) {
        if (listItem->getCommand() == "label") {
            if (listItem->getArg().length() > 0) {
                labelList[listItem->getArg()] = listItem->getNext();
            } else {
                cout << "ERR: Line " << listItem->getLineNumber()
                     << " - 'label' requires an argument.\n"
                     << "JAZ: This label will be ignored.\n\n";
            }
        }
        listItem = listItem->getNext();
    }
    
    //unfortunately, there's no for_each algorithm for linked lists, so we need
    //to run the logic on the final list item separately.  Ideally, the .jaz
    //file should not end with a label, and it absolutely should not be called,
    //but we'll catch that error, if it comes up, in executer.
    if (listItem->getCommand() == "label") {
        if (listItem->getCommand() == "label") {
            if (listItem->getArg().length() > 0) {
                labelList[listItem->getArg()] = listItem->getNext();
                cout << "ERR: Line " << listItem->getLineNumber()
                     << " - 'label " << listItem->getArg()
                     << "' is placed at the last line of the file.\n"
                     << "JAZ: Any jumps to this label will be ignored.\n\n";
            } else {
                cout << "ERR: Line " << listItem->getLineNumber()
                     << "-  'label' requires an argument.\n"
                     << "JAZ: This label will be ignored.\n\n";
            }
        }
    }
    return labelList;
}

//rather than stick this (rather ugly) code into the middle of buildList, I put
//it here to be called.  It takes each line of the.jaz file and simply splits it
//into a command and a single argument, removing any whitespace other than
//spaces inside the argument (in order for "show" to work correctly.
pair<string, string> Reader::parseLine(string line) {
    pair<string, string> out = make_pair("", "");
    
    //white space symbols to ignore.
    string parseSymbols(" \t\r");
    //size_t's for finding positions of words in the line.
    size_t commandBegin = 0,
           commandEnd = 0,
           argBegin = 0,
           argEnd = 0;
    
    commandBegin = line.find_first_not_of(parseSymbols);
    //Test if the line was blank
    if (commandBegin != string::npos) {
        commandEnd = line.find_first_of(parseSymbols, commandBegin);
        //we've encapsulated our single word command, place it in output.
        out.first = line.substr(commandBegin, commandEnd - commandBegin);
        
        argBegin = line.find_first_not_of(parseSymbols, commandEnd);
        //test if there was no argument
        if (argBegin != string::npos) {
            argEnd = line.find_last_not_of(parseSymbols);
            //we've found our full argument, put it in output.
            out.second = line.substr(argBegin, (argEnd - argBegin) + 1);
        }
    }
    return out;
}
