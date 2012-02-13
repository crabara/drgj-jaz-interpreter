#include "Executer.h"

Executer::Executer() {
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
    return instrList;
}

map<string, InstructionItem*> Executer::getLabelMap() {
    return labelMap;
}

void Executer::execute(ofstream& os, bool fileIsOpen) {
  while (true) {
    int lineNumber = instrList->getLineNumber();
    string command = instrList->getCommand();
    string arg = instrList->getArg();
    
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
            if (variableList.count(arg)) {
                memoryStack.push(variableList[arg]);
            } else {
                cout << "ERR: Line - " << lineNumber
                     << " - 'rvalue " << arg << "' does not exist.\n"
                     << "JAZ: RVALUE operation cannot be performed.\n"
                     << "JAZ: Halting execution.\n\n";
                break;
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
        if (!memoryStack.empty()) {
            memoryStack.pop();
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: POP operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == ":=") {
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            variableList[stackTop.first] = stackTop.second;
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: ASSIGNMENT operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
    }
    
    else if (command == "copy") {
        string temp("0");
        if (!memoryStack.empty()) {
            temp = memoryStack.top();
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: COPY operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(temp);
    }
    
    else if (command == "label") { }
    
    else if (command == "goto") {
        if (arg.length() > 0) {
            if (labelMap.count(arg)) {
                instrList->setNext(labelMap[arg]);
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
                        instrList->setNext(labelMap[arg]);
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
                        instrList->setNext(labelMap[arg]);
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
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            sum = atoi(stackTop.first.c_str())
                    + atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: ADD operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(sum));
    }
    
    else if (command == "-") {
        int diff = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            diff = atoi(stackTop.first.c_str())
                    - atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: SUBTRACT operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(diff));
    }
    
    else if (command == "*") {
        int prod = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            prod = atoi(stackTop.first.c_str())
                    * atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: MULTIPLY operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(prod));
    }
    
    else if (command == "/") {
        int quot = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            quot = atoi(stackTop.first.c_str())
                    / atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: DIVIDE operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(quot));
    }
    
    else if (command == "div") {
        int rem = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            rem = atoi(stackTop.first.c_str())
                    % atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: MODULO operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(rem));
    }
    
    else if (command == "&") {
        int result = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            result = atoi(stackTop.first.c_str())
                       && atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: AND operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(result));
    }
    
    else if (command == "!") {
        int operand = 0,
            result = 0;
        if (!memoryStack.empty()) {
            operand = atoi(memoryStack.top().c_str());
            memoryStack.pop();
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: NOT operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        
        result = !operand;
        memoryStack.push(convertInt(result));
    }
    
    else if (command == "|") {
        int result = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            result = atoi(stackTop.first.c_str())
                       || atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: OR operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(result));
    }
    
    else if (command == "<>") {
        int result = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            result = atoi(stackTop.first.c_str())
                       != atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: NOT EQUAL operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(result));
    }
    
    else if (command == "<=") {
        int result = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            result = atoi(stackTop.first.c_str())
                       <= atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: LESS OR EQUAL operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(result));
    }
    
    else if (command == ">=") {
        int result = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            result = atoi(stackTop.first.c_str())
                       >= atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: GREATER OR EQUAL operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(result));
    }
    
    else if (command == "<") {
        int result = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            result = atoi(stackTop.first.c_str())
                       < atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: LESS THAN operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(result));
    }
    
    else if (command == ">") {
        int result = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            result = atoi(stackTop.first.c_str())
                       > atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: GREATER THAN operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(result));
    }
    
    else if (command == "=") {
        int result = 0;
        pair<string, string> stackTop = popTwo();
        if (stackTop.first.length() > 0) {
            result = atoi(stackTop.first.c_str())
                       == atoi(stackTop.second.c_str());
        } else {
            cout << "ERR: Line " << lineNumber
                 << " - Stack is empty.\n"
                 << "JAZ: EQUAL operation cannot be performed.\n"
                 << "JAZ: Halting execution.\n\n";
            break;
        }
        memoryStack.push(convertInt(result));
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
        
    }
    
    else if (command == "end") {
        
    }
    
    else if (command == "return") {
        if (!returnToCaller.empty()) {
            instrList->setNext(returnToCaller.top());
            returnToCaller.pop();
        }
    }
    
    else if (command == "call") {
        if (arg.length() > 0) {
            returnToCaller.push(instrList->getNext());
            if (labelMap.count(arg)) {
                instrList->setNext(labelMap[arg]);
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
    
    if (instrList->getNext() != NULL) {
        instrList = instrList->getNext();
    } else {
        cout << "ERR: Line " << lineNumber
             << " - Moved past end of file with no HALT command.\n"
             << "JAZ: Halting execution.\n\n";
        break;
    }
  }
}

pair<string, string> Executer::popTwo() {
    pair<string, string> outPair = make_pair("", "");
    if (!memoryStack.empty()) {
        outPair.second = memoryStack.top();
        memoryStack.pop();
    }
    if (!memoryStack.empty()) {
        outPair.first = memoryStack.top();
        memoryStack.pop();
    }
    return outPair;
}

string Executer::convertInt(int intToConvert) {
    char temp[100];
    sprintf(temp, "%d", intToConvert);
    string result(temp);
    return result;
}
