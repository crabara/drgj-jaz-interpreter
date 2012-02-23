#include "Reader.h"

Reader::Reader() {
    this->lineTracker = 0;
}

Reader::~Reader() {
}

//buildList recurses over each line of the jaz file until it reaches the end.
//It then collapses backwards, returning pointers to each item in the list
//until it returns a pointer to the head of the list to whatever called it in
//the first place.
InstructionItem* Reader::buildList(ifstream& is) {
    //update the line number.
    lineTracker++;
    InstructionItem* listItem = NULL;
    
    //pull in a line from the jaz file and convert it into a string.
    char holder[100];
    is.getline(holder, 100);
    string line(holder);
    
    //parse the line into a command and an argument.
    pair<string, string> instrPair = parseLine(line);
    
    //check to see if the line was blank.
    if (instrPair.first.length() == 0) {
        
        if (!is.eof()) {
            //the line is blank, but there are more lines to parse.
            listItem = buildList(is);
        }
        
    } else {
        //the line is useful, put it at the end of the linked list.
        listItem = new InstructionItem(instrPair.first,
                                       instrPair.second,
                                       lineTracker);
        
        if (!is.eof()) {
            //there are more lines to parse, continue on.
            listItem->setNext(buildList(is));
        }
    }
    return listItem;
}

//this method simply parses a line into two parts, the command and the argument.
pair<string, string> Reader::parseLine(string line) {
    pair<string, string> out = make_pair("", "");
    
    //these are the characters that should be ignored, space, tab and return.
    string parseSymbols(" \t\r");
    
    //these size_t's are used to track and remove superfluous white space from
    //the line.
    size_t commandBegin = 0,
           commandEnd = 0,
           argBegin = 0,
           argEnd = 0;
    
    //find the first character in the line that is not white space.
    commandBegin = line.find_first_not_of(parseSymbols);
    
    //check to see if the line is blank.
    if (commandBegin != string::npos) {
        //find the end of the command.
        commandEnd = line.find_first_of(parseSymbols, commandBegin);
        //copy the command to the first part of the pair.
        out.first = line.substr(commandBegin, commandEnd - commandBegin);
        
        //check to see if this line is a show command (a special case).
        if (out.first == "show") {
            //show means the entirety of the rest of the line is the argument.
            //This includes all the whitespace.
            argBegin = commandEnd + 1;
            out.second = line.substr(argBegin);
            
        } else {
            //find the first character of the argument.
            argBegin = line.find_first_not_of(parseSymbols, commandEnd);
            
            //test if this line does not have an argument.
            if (argBegin != string::npos) {
                //find the end of the command.
                argEnd = line.find_first_of(parseSymbols, argBegin);
                //copy the argument to the second part of the pair.
                out.second = line.substr(argBegin, argEnd - argBegin);
            }
        }
    }
    
    //send back the parsed line.
    return out;
}
