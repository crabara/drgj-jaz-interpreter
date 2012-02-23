#include "Executer.h"

Executer::Executer() {
    this->wideScope = new SymbolTable;
    this->narrowScope = wideScope;
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

//Here is the meat of the program, each command is explained below.
void Executer::execute(ofstream& os, bool fileIsOpen) {
  //run until a break.
  while (true) {
    //Pull in information on the current instruction item.  This makes working
    //with the instruction simpler.
    string command = instrList->getCommand();
    string arg = instrList->getArg();
    InstructionItem* nextInstr = instrList->getNext();
    
    //Pull in the next instruction, any branching statement (goto, gotrue,
    //gofalse, call, return) will change this value.
    instrList->incrementAccess();
    
    //push simply pushes the argument onto the stack.
    if (command == "push") {
        if (arg.length() > 0) {
            memoryStack.push(arg);
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "push requires an argument.");
            haltMessage(command);
            break;
        }
    }
    
    //rvalue checks for a variable named after the argument and pushes the value
    //of that variable onto the stack.
    else if (command == "rvalue") {
        if (arg.length() > 0) {
            string first = wideScope->getVariable(arg);
            if (first.length() > 0) {
                memoryStack.push(first);
            } else {
                memoryStack.push("0");
            }
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "rvalue requires an argument.");
            haltMessage(command);
            break;
        }
    }
    
    //lvalue pushes the argument onto the stack.  This will be used for a later
    //variable assigment.
    else if (command == "lvalue") {
        if (arg.length() > 0) {
            memoryStack.push(arg);
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "lvalue requires an argument.");
            haltMessage(command);
            break;
        }
    }
    
    //pop pops the top of the stack and discards it.
    else if (command == "pop") {
        string first = popTop();
        if (first.length() == 0) {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Assign pulls the top two elements of the stack and declares a variable.
    //the top element is the value and the second element is the variable name.
    else if (command == ":=") {
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            narrowScope->setVariable(first, second);
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //copy simply places a copy of the top of the stack on top of the stack.
    else if (command == "copy") {
        if (!memoryStack.empty()) {
            memoryStack.push(memoryStack.top());
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //label has already been handled, no further processing is necessary.
    else if (command == "label") { }
    
    //goto changes the next instruction to execute.  The instruction is resolved
    //using the label provided by the argument.
    else if (command == "goto") {
        if (arg.length() > 0) {
            if (labelMap.count(arg)) {
                nextInstr = labelMap[arg];
            } else {
                errMessage(instrList->getLineNumber(), instrList->getAccess(),
                           "label " + arg + " does not exist.");
                haltMessage(command);
                break;
            }
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "goto requires an argument.");
            haltMessage(command);
            break;
        }
    }
    
    //gofalse does the same as goto, but only if the top of the stack is a 0.
    else if (command == "gofalse") {
        int decider = 0;
        if (arg.length() > 0) {
            if (!memoryStack.empty()) {
                decider = atoi(memoryStack.top().c_str());
                memoryStack.pop();
                if (decider == 0) {
                    if (labelMap.count(arg)) {
                        nextInstr = labelMap[arg];
                    } else {
                        errMessage(instrList->getLineNumber(),
                                   instrList->getAccess(),
                                   "label " + arg + " does not exist.");
                        haltMessage(command);
                        break;
                    }
                }
            } else {
                errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
                haltMessage(command);
                break;
            }
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "gofalse requires an argument.");
            haltMessage(command);
            break;
        }
    }
    
    
    //gotrue does the same as goto, but only if the top of the stack is not a 0.
    else if (command == "gotrue") {
        int decider = 0;
        if (arg.length() > 0) {
            if (!memoryStack.empty()) {
                decider = atoi(memoryStack.top().c_str());
                memoryStack.pop();
                if (decider != 0) {
                    if (labelMap.count(arg)) {
                        nextInstr = labelMap[arg];
                    } else {
                        errMessage(instrList->getLineNumber(),
                                   instrList->getAccess(),
                                   "label " + arg + " does not exist.");
                        haltMessage(command);
                        break;
                    }
                }
            } else {
                errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
                haltMessage(command);
                break;
            }
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "gotrue requires an argument.");
            haltMessage(command);
            break;
        }
    }
    
    //The jaz file execution has been completed.
    else if (command == "halt") {
        cout << "\nJAZ: Execution finished.\n";
        break;
    }
    
    //Adds the top two elements of the stack.
    else if (command == "+") {
        int sum = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            sum = atoi(first.c_str()) + atoi(second.c_str());
            memoryStack.push(convertInt(sum));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Subtracts the top element of the stack by the second element.
    else if (command == "-") {
        int diff = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            diff = atoi(first.c_str()) - atoi(second.c_str());
            memoryStack.push(convertInt(diff));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Multiplies the top two elements of the stack.
    else if (command == "*") {
        int prod = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            prod = atoi(first.c_str()) * atoi(second.c_str());
            memoryStack.push(convertInt(prod));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Divides the second element of the stack by the top element.
    else if (command == "/") {
        int quot = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            if (second != "0") {
                quot = atoi(first.c_str()) / atoi(second.c_str());
                memoryStack.push(convertInt(quot));
            } else {
                errMessage(instrList->getLineNumber(), instrList->getAccess(),
                           "attempted divide by zero.");
                haltMessage(command);
                break;
            }
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Performs a modulo using the second element of the stack divided by the
    //top element and pushing the remainder on the stack.
    else if (command == "div") {
        int rem = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            if (second != "0") {
                rem = atoi(first.c_str()) % atoi(second.c_str());
                memoryStack.push(convertInt(rem));
            } else {
                errMessage(instrList->getLineNumber(), instrList->getAccess(),
                           "attempted divide by zero.");
                haltMessage(command);
                break;
            }
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Logical AND of the top two elements on the stack.
    else if (command == "&") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) && atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Logical NOT of the top element of the stack.
    else if (command == "!") {
        int result = 0;
        string first = popTop();
        if (first.length() > 0) {
            result = !(atoi(first.c_str()));
            memoryStack.push(convertInt(result));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Logical OR of the top two elements on the stack.
    else if (command == "|") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) || atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Tests if the top two elements on the stack are not equal.
    else if (command == "<>") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) != atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Tests if the second element on the stack is less than or equal to the top
    //element of the stack.
    else if (command == "<=") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) <= atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Tests if the second element on the stack is greater than or equal to the
    //top element of the stack.
    else if (command == ">=") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) >= atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Tests if the second element on the stack is less than the top element of
    //the stack.
    else if (command == "<") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) < atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Tests if the second element on the stack is greater than the top element
    //of the stack.
    else if (command == ">") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) > atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Tests if the second element on the stack equals the top element of the
    //stack.
    else if (command == "=") {
        int result = 0;
        string second = popTop();
        string first = popTop();
        if (first.length() > 0) {
            result = atoi(first.c_str()) == atoi(second.c_str());
            memoryStack.push(convertInt(result));
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Prints the top element of the stack to the command window and output file.
    else if (command == "print") {
        if (!memoryStack.empty()) {
            string temp = memoryStack.top();
            cout << temp << "\r\n";
            if(fileIsOpen) {
                os << temp << "\r\n";
            }
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "stack is currently empty.");
            haltMessage(command);
            break;
        }
    }
    
    //Prints the argument of the show command to the command window and output
    //file.
    else if (command == "show") {
        cout << arg << "\r\n";
        if (fileIsOpen) {
            os << arg << "\r\n";
        }
    }
    
    //Changes the scope of the program in preparation for a function call.
    else if (command == "begin") {
        narrowScope = new SymbolTable;
        symbolTables.push(wideScope);
    }
    
    //Restores the scope of the program after a function has returned.
    else if (command == "end") {
        if (!symbolTables.empty()) {
            wideScope = narrowScope;
            symbolTables.pop();
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "end attempted before using begin.");
            haltMessage(command);
            break;
        }
    }
    
    //Prepares the program to take in the values calculated during a subroutine.
    //Also returns the execution to the correct line in the Instruction list.
    else if (command == "return") {
        if (!returnToCaller.empty()) {
            narrowScope = symbolTables.top();
            nextInstr = returnToCaller.top();
            returnToCaller.pop();
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "return attempted before using call.");
            haltMessage(command);
            break;
        }
    }
    
    //Prepares the program to send variables to the called function.  Also
    //pushes the next instruction onto the returnToCaller stack so the return
    //command knows where to come back to.
    else if (command == "call") {
        if (arg.length() > 0) {
            wideScope = narrowScope;
            returnToCaller.push(nextInstr);
            if (labelMap.count(arg)) {
                nextInstr = labelMap[arg];
            } else {
                errMessage(instrList->getLineNumber(), instrList->getAccess(),
                           "label " + arg + " does not exist.");
                haltMessage(command);
                break;
            }
        } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "call requires an argument.");
            haltMessage(command);
            break;
        }
    }
    
    //This command is not part of the machine specifications or misspelled.
    else {
        errMessage(instrList->getLineNumber(), instrList->getAccess(),
                   command + " is not a valid command.");
        haltMessage(command);
        break;
    }
    
    //The user has written the jaz file improperly, leaving out a halt command
    //or calling a label that passes the end of the file.
    if (nextInstr != NULL) {
        instrList = nextInstr;
    } else {
            errMessage(instrList->getLineNumber(), instrList->getAccess(),
                       "Moved past end of file with no halt command.");
            cout << "JAZ: Halting execution.\n\n";
            break;
    }
  }
}

//popTop returns the top of the stack, if it isn't empty, or a NULL string, if
//it is.
string Executer::popTop() {
    string top("");
    if (!memoryStack.empty()) {
        top = memoryStack.top();
        memoryStack.pop();
    }
    return top;
}

//The itoa() function is not standardized across all C++ libraries.  To that
//end, convertInt will convert an integer to a string using a temporary c-string
// and the sprintf() function.
string Executer::convertInt(int intToConvert) {
    char temp[100];
    sprintf(temp, "%d", intToConvert);
    string result(temp);
    return result;
}

//errMessage outputs what went wrong during execution in an attempt to help the
//writer of the jaz file pinpoint their errors.
void Executer::errMessage(int lineNumber, int access, string problem) {
    cout << "\nERR: Line " << lineNumber << " - " << problem << endl
         << "ERR: Line " << lineNumber << " has been executed "
         << access << " time(s).\n\n";
}

//If a situation arises where execution could not possibly continue, haltMessage
//will let the user know.
void Executer::haltMessage(string command) {
    cout << "JAZ: " << command << " could not be completed.\n"
         << "JAZ: Halting execution.\n\n";
}
