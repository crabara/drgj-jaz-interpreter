#include "Reader.h"

Reader::Reader() {
    this->lineTracker = 0;
}

Reader::~Reader() {
}

InstructionItem* Reader::buildList(ifstream& is) {
    lineTracker++;
    InstructionItem* listItem = NULL;
    char holder[100];
    is.getline(holder, 100);
    string line(holder);
    pair<string, string> instrPair = parseLine(line);
    if (instrPair.first.length() == 0) {
        if (!is.eof()) {
            listItem = buildList(is);
        }
    } else {
        listItem = new InstructionItem(instrPair.first,
                                       instrPair.second,
                                       lineTracker);
        if (!is.eof()) {
            listItem->setNext(buildList(is));
        }
    }
    return listItem;
}

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

pair<string, string> Reader::parseLine(string line) {
    pair<string, string> out = make_pair("", "");
    string parseSymbols(" \t\r");
    size_t commandBegin = 0,
           commandEnd = 0,
           argBegin = 0,
           argEnd = 0;
    commandBegin = line.find_first_not_of(parseSymbols);
    if (commandBegin != string::npos) {
        commandEnd = line.find_first_of(parseSymbols, commandBegin);
        out.first = line.substr(commandBegin, commandEnd - commandBegin);
        argBegin = line.find_first_not_of(parseSymbols, commandEnd);
        if (argBegin != string::npos) {
            argEnd = line.find_last_not_of(parseSymbols);
            out.second = line.substr(argBegin, (argEnd - argBegin) + 1);
        }
    }
    return out;
}
