/* 
 * File:   main.cpp
 * Author: Jag
 *
 * Created on February 11, 2012, 5:42 AM
 */

#include "Executor.h"
#include "Reader.h"
#include "InstructionItem.h"
#include <iostream>

using namespace std;

void myfunction(pair<string, InstructionItem*> p) {
    cout << p.first << " " << p.second << endl;
}

int main(int argc, char** argv) {
    //Just testing code right now.  This shows me that the linked list and
    //reader functions are working correctly.
    ifstream is("foo.jaz");
    InstructionItem* listHead = Reader::buildList(is);
    map<string, InstructionItem*> labelLocs = Reader::mapLabels(listHead);
    
    while (listHead->getNext() != NULL) {
        cout << listHead->getCommand() << " " << listHead->getArg() << endl;
        listHead = listHead->getNext();
    }
    cout << listHead->getCommand() << " " << listHead->getArg() << endl;
    
    cout << "Press ENTER to exit.";
    getchar();

    return 0;
}

