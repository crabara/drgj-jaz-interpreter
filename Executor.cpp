#include "Executer.h"
#include "SymbolTable.h"

Executer::Executer() {
    this->wideScope = &this->variableList;
    this->narrowScope = &this->variableList;
}

Executer::~Executer() {
}

void Executer::setInstructionList(InstructionItem* listHead) {
    this->instrList = listHead;
}

void Executer::setLabelMap(map<string,InstructionItem*> labelMap) {
    this->labelMap = labelMap;
}

InstructionItem* Executer::getCurrentInstruction() {
    return this->instrList;
}

map<string, InstructionItem*> Executer::getLabelMap() {
    return this->labelMap;
}

void Executer::execute(ofstream& os, bool fileIsOpen) {
  while (true) {
    int lineNumber = instrList->getLineNumber();
    string command = instrList->getCommand();
    string arg = instrList->getArg();
    instrList->incrementAccess();
    InstructionItem* nextInstr = NULL;
    bool conditionalJump = false;
    
    if (command == "push") {
        if (arg.length() > 0) {
            memoryStack.push(arg);
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - 'push' requires an argument.\n"
                 << "JAZ: PUSH operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "rvalue") {
        if (arg.length() > 0) {
            string first = wideScope->getVariable(arg);
            if (first.length() > 0) {
                memoryStack.push(first);
            } else {
                memoryStack.push("0");
            }
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - 'rvalue' requires an argument.\n"
                 << "JAZ: RVALUE operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "lvalue") {
        if (arg.length() > 0) {
            memoryStack.push(arg);
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - 'lvalue' requires an argument.\n"
                 << "JAZ: LVALUE operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "pop") {
        string first = popTop();
        if (first.length() == 0) {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: POP operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == ":=") {
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            narrowScope->setVariable(first, second);
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: ASSIGNMENT operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "copy") {
        if (!memoryStack.empty()) {
            memoryStack.push(memoryStack.top());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: COPY operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "label") { }
    
    else if (command == "goto") {
        if (arg.length() > 0) {
            if (labelMap.count(arg)) {
                nextInstr = labelMap[arg];
                conditionalJump = true;
            } else {
                cout << "ERR: Line " << lineNumber
                     << " - 'label " << arg << "' does not exist.\n"
                     << "JAZ: GOTO operation cannot be performed.\n"
                     << "JAZ: Halting execution.\n\n";
                break;
            }
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - 'goto' requires an argument.\n"
                 << "JAZ: GOTO operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "gofalse") {
        int decider = 0;
        if (arg.length() > 0) {
            if (!memoryStack.empty()) {
                decider = atoi(memoryStack.top().c_str());
                memoryStack.pop();
                if (decider == 0) {
                    if (labelMap.count(arg)) {
                        nextInstr = labelMap[arg];
                        conditionalJump = true;
                    } else {
                        cout << "ERR: Line " << lineNumber
                             << " - 'label " << arg << "' does not exist.\n"
                             << "JAZ: GOFALSE operation cannot be performed.\n"
                             << "JAZ: Halting execution.\n\n";
                        break;
                    }
                }
            } else {
                cout << "ERR: Line " << lineNumber
                     << " - Stack is empty.\n"
                     << "JAZ: GOFALSE operation cannot be performed.\n"
                     << "JAZ: Halting execution.\n\n";
                break;
            }
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - 'gofalse' requires an argument.\n"
                 << "JAZ: GOFALSE operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "gotrue") {
        int decider = 0;
        if (arg.length() > 0) {
            if (!memoryStack.empty()) {
                decider = atoi(memoryStack.top().c_str());
                memoryStack.pop();
                if (decider != 0) {
                    if (labelMap.count(arg)) {
                        nextInstr = labelMap[arg];
                        conditionalJump = true;
                    } else {
                        cout << "ERR: Line " << lineNumber
                             << " - 'label " << arg << "' does not exist.\n"
                             << "JAZ: GOTRUE operation cannot be performed.\n"
                             << "JAZ: Halting execution.\n\n";
                             break;
                    }
                }
            } else {
                cout << "ERR: Line " << lineNumber
                     << " - Stack is empty.\n"
                     << "JAZ: GOTRUE operation cannot be performed.\n"
                     << "JAZ: Halting execution.\n\n";
                break;
            }
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - 'gotrue' requires an argument.\n"
                 << "JAZ: GOTRUE operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "halt") {
        cout << "JAZ: Execution finished.\n";
        break;
    }
    
    else if (command == "+") {
        int sum = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            sum = atoi(first.c_str()) + atoi(second.c_str());
            memoryStack.push(convertInt(sum));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: ADD operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "-") {
        int diff = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            diff = atoi(first.c_str()) - atoi(second.c_str());
            memoryStack.push(convertInt(diff));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: SUBTRACT operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "*") {
        int prod = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            prod = atoi(first.c_str()) * atoi(second.c_str());
            memoryStack.push(convertInt(prod));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: MULTIPLY operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "/") {
        int quot = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            quot = atoi(first.c_str()) / atoi(second.c_str());
            memoryStack.push(convertInt(quot));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: DIVIDE operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "div") {
        int rem = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            rem = atoi(first.c_str()) % atoi(second.c_str());
            memoryStack.push(convertInt(rem));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: MODULO operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "&") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) && atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: AND operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "!") {
        int result = 0;
        string first = popTop();
        if (first.length() > 0) {
            result = !(atoi(first.c_str()));
            memoryStack.push(convertInt(result));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: NOT operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "|") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) || atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: OR operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "<>") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) != atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: NOT EQUAL operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "<=") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) <= atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: LESS OR EQUAL operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == ">=") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) >= atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: GREATER OR EQUAL operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "<") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) < atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: LESS THAN operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == ">") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) > atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: GREATER THAN operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "=") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) == atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: EQUAL operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "print") {
        if (!memoryStack.empty()) {
            string temp = memoryStack.top();
            cout << temp << "\r\n";
            if(fileIsOpen) {
                os << temp << "\r\n";
            }
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: PRINT operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "show") {
        cout << arg << "\r\n";
        if (fileIsOpen) {
            os << arg << "\r\n";
        }
    }
    
    else if (command == "begin") {
        narrowScope = new SymbolTable;
        symbolTables.push(wideScope);
    }
    
    else if (command == "end") {
        wideScope = narrowScope;
        symbolTables.pop();
    }
    
    else if (command == "return") {
        if (!returnToCaller.empty()) {
            narrowScope = symbolTables.top();
            instrList->setNext(returnToCaller.top());
            returnToCaller.pop();
        }
    }
    
    else if (command == "call") {
        if (arg.length() > 0) {
            wideScope = narrowScope;
            returnToCaller.push(instrList->getNext());
            if (labelMap.count(arg)) {
                nextInstr = labelMap[arg];
                conditionalJump = true;
            } else {
                cout << "ERR: Line - " << lineNumber
                     << " - 'label " << arg << "' does not exist.\n"
                     << "JAZ: CALL operation cannot be performed.\n"
                     << "JAZ: Halting execution.\n\n";
                break;
            }
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - 'call' requires an argument.\n"
                 << "JAZ: CALL operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else {
        cout << "ERR: Line " << lineNumber
             << " - '" << command << "' is not a valid command.\n"
             << "JAZ: Halting execution.\n\n";
        break;
    }
    
    if (!conditionalJump) {
        if (instrList->getNext() != NULL) {
            instrList = instrList->getNext();
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Moved past end of file with no HALT command.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    } else {
        instrList = nextInstr;
    }
  }
}

string Executer::popTop() {
    string top("");
    if (!memoryStack.empty()) {
        top = memoryStack.top();
        memoryStack.pop();
    }
    return top;
}

string Executer::convertInt(int intToConvert) {
    char temp[100];
    sprintf(temp, "%d", intToConvert);
    string result(temp);
    return result;
}
