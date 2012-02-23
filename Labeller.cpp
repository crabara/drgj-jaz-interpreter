#include "Labeller.h"

Labeller::Labeller() { }

Labeller::~Labeller() { }

//mapLabels is pretty simple, it just looks through the entire instruction list
//for label commands and associates those labels with the place in the
//instruction list that label points to.
map<string, InstructionItem*> Labeller::mapLabels(InstructionItem* listHead) {
    map<string, InstructionItem*> labelList;
    
    //iterate through the list until the end is reached.
    while (listHead->getNext() != NULL) {
        //check to see if the label command is invoked.
        if (listHead->getCommand() == "label") {
            //check to see that an argument was provided.
            if (listHead->getArg().length() > 0) {
                //associate the label with the instruction it points to.
                labelList[listHead->getArg()] = listHead->getNext();
                
            } else {
                //the label was written improperly.
                cout << "ERR: Line " << listHead->getLineNumber()
                     << " - label requires an argument.\n"
                     << "JAZ: This label will be ignored.\n\n";
            }
        }
        
        //move to the next item in the instruction list.
        listHead = listHead->getNext();
    }
    
    //perform the same checks for the last item in the list.
    if (listHead->getCommand() == "label") {
        if (listHead->getCommand() == "label") {
            if (listHead->getArg().length() > 0) {
                labelList[listHead->getArg()] = listHead->getNext();
                
                //ideally, the last item in the jaz file should not be a label.
                //if this label is called at any point, it will result in an
                //error.  This is handled during execution.
                cout << "ERR: Line " << listHead->getLineNumber()
                     << " - label " << listHead->getArg()
                     << " is placed at the last line of the file.\n"
                     << "JAZ: Any jumps to this label will halt execution.\n\n";
                
            } else {
                //again, the label was written without an argument.
                cout << "ERR: Line " << listHead->getLineNumber()
                     << "-  label requires an argument.\n"
                     << "JAZ: This label will be ignored.\n\n";
            }
        }
    }
    
    return labelList;
}
